/*
	Name: AstFileReader.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 25/04/25 18:55
	Description: AST编码读取器
*/

#pragma once

#include "Ast.hpp"
#include "AstFileReaderException.cpp"
#include "BufferStream.cpp"
#include "Stack.hpp"
#include "String.hpp"
#include "TypeCalculator.cpp"

// 用于简写的宏
#define CE \
	CATCH { \
		return NULL; \
	}

namespace stamon::action {
class AstFileReader {
	BufferInStream stream;

public:
	STMException *ex;

	AstFileReader(STMException *e, const BufferInStream &instream)
		: ex(e)
		, stream(instream) {
		ex = e;

		byte magic_number[2];
		stream.readArray(magic_number);

		CATCH {
			return;
		}

		if (magic_number[0] != (byte) 0xAB || magic_number[1] != (byte) 0xDD) {
			// 如果文件过小或魔数异常则报错
			THROW(exception::astfilereader::FormatError("start"));
			return;
		}
	}

	int updateLineNo() {
		// 更新行号
		int rst;

		stream.read(rst);

		CATCH {
			return -1;
		}

		return rst;
	}

	String updateFileName() {
		int len;

		stream.read(len);

		CATCH {
			return String();
		}

		if (len < 0) {
			THROW(exception::astfilereader::FormatError("updateFileName()"));
			return String();
		}

		byte *cstr = new byte[len];

		for (int i = 0; i < len; i++) {
			stream.read(cstr[i]);
		}

		String filename = String(cstr);

		delete[] cstr;

		return filename;
	}

	ast::AstNode *readNode(int type) {
		ast::AstNode *n = NULL;

		if (type == -1) {
			return NULL;
			// 返回结尾单元
		}

		if (type >= ast::AstTypeNum) {
			// 非法AST编号
			THROW(exception::astfilereader::NodeError("readNode()"));
			return NULL;
		}

		// 对有ast数据的节点进行特判
		switch (type) {
		case ast::AstIdentifierType: {
			// 标识符
			int len;
			stream.read(len);
			CE;

			if (len < 0) {
				THROW(exception::astfilereader::FormatError("readNode()"));
				return NULL;
			}

			// 再读取到byte数组里
			byte *cstr = new byte[len + 1];
			cstr[len] = '\0';

			for (int i = 0; i < len; i++) {
				stream.read(cstr[i]);
			}
			CE;

			String iden(cstr, len);
			delete cstr; // 释放内存
			n = new ast::AstIdentifierName(iden);
			break;
		}

		case ast::AstNumberType: {
			// 数字
			byte numtype;
			stream.read(numtype);
			CE;

			switch (numtype) {
			case ast::IntNumberType: {
				// 整数
				int val;
				stream.read(val);
				CE;
				n = new ast::AstIntNumber(val);
				break;
			}
			case ast::FloatNumberType: {
				// 单精度浮点
				float val;
				stream.read(val);
				CE;
				n = new ast::AstFloatNumber(val);
				break;
			}
			case ast::DoubleNumberType: {
				// 双精度浮点
				double val;
				stream.read(val);
				CE;
				n = new ast::AstDoubleNumber(val);
				break;
			}
			}
			break;
		}

		case ast::AstStringType: {
			// 字符串
			// 先读取出字符串长度
			int len;
			stream.read(len);
			CE;

			if (len < 0) {
				THROW(exception::astfilereader::FormatError("readNode()"));
				return NULL;
			}

			// 再读取到byte数组里
			byte *cstr = new byte[len + 1];
			cstr[len] = '\0';

			for (int i = 0; i < len; i++) {
				stream.read(cstr[i]);
			}
			CE;

			String val(cstr, len);
			delete cstr; // 释放内存
			n = new ast::AstString(val);
			break;
		}

		case ast::AstAnonClassType: {
			// 匿名类
			byte father_flag;
			stream.read(father_flag);
			CE;

			n = new ast::AstAnonClass();
			((ast::AstAnonClass *) n)->father_flag = father_flag;
			break;
		}

		case ast::AstExpressionType: {
			// 表达式
			int ass_type;
			stream.read(ass_type);
			CE;

			if (vm::TypeCalculator().AssignOperatorToBinaryOperator(ass_type)
					!= -1) {
				// 判断赋值编号是否合法
				THROW(exception::astfilereader::FormatError("readNode()"));
				return NULL;
			}

			n = new ast::AstExpression();
			((ast::AstExpression *) n)->ass_type = ass_type;
		}

		case ast::AstBinaryType: {
			int operator_type;
			stream.read(operator_type);
			CE;

			if (vm::TypeCalculator().checkBinaryOperator(operator_type)
					== false) {
				// 判断运算符编号是否合法
				THROW(exception::astfilereader::FormatError("readNode()"));
				return NULL;
			}

			n = new ast::AstBinary();
			((ast::AstBinary *) n)->operator_type = operator_type;
		}

		case ast::AstUnaryType: {
			int operator_type;
			stream.read(operator_type);
			CE;

			if (vm::TypeCalculator().checkUnaryOperator(operator_type)
					== false) {
				// 判断运算符编号是否合法
				THROW(exception::astfilereader::FormatError("readNode()"));
				return NULL;
			}

			n = new ast::AstUnary();
			((ast::AstUnary *) n)->operator_type = operator_type;
		}

		case ast::AstPostfix: {
			int postfix_type;
			stream.read(postfix_type);
			CE;

			if ((0 <= postfix_type && postfix_type < ast::PostfixTypeCount)
					== false) {
				// 判断后缀编号是否合法
				THROW(exception::astfilereader::FormatError("readNode()"));
				return NULL;
			}

			n = new ast::AstPostfix();
			((ast::AstPostfix *) n)->postfix_type = postfix_type;
		}
		}

		if (n == NULL) {
			// 非特殊节点
			n = new ast::AstNode(type);
			THROW(exception::astfilereader::NodeError("readNode()"));
		}

		return n;
	}

	ast::AstNode *read() {
		ArrayList<ast::AstNode *> flatAstNode;
		// 平面的节点（包括结尾单元）

		int lineno = -1;
		String filename;

		while (stream.isMore()) {
			byte type;
			stream.read(type);
			CE;

			while (type == -2 || type == -3) {
				if (type == -2) {
					lineno = updateLineNo();
				}
				if (type == -3) {
					filename = updateFileName();
				}
				stream.read(byte);
				CE;
			}

			ast::AstNode *node = readNode(type);

			CATCH {
				return NULL;
			}

			if (node != NULL) {
				// 非结尾单元需指定行号和文件名
				node->lineNo = lineno;
				node->filename = filename;
			}

			flatAstNode.add(node);
		}

		// 将平面的节点复原成树状结构

		Stack<ast::AstNode*> stack;

		ast::AstNode *root = NULL;

		if (flatAstNode.size() == 0) {
			THROW(exception::astfilereader::RootNodeError("read()"));
			return root;
		}

		for (int i = 0, len = ir.size(); i < len; i++) {
			if (flatAstNode[i] == NULL) {
				// 特判结束符

				if (stack.empty()) {
					// 多余的终结符
					THROW(exception::astfilereader::RedundantRootNodeError(
							"read()"));
					return root;
				}

				stack.pop();
				continue;
			}

			// 创建一个节点
			ast::AstNode *n = flatAstNode[i];

			if (stack.empty()) {
				// 该节点是根节点

				if (root != NULL) {
					// 已经出现根节点了，因此是重复的根节点
					THROW(exception::astfilereader::RedundantRootNode(
							"read()"));
					return root;
				}

				root = n;

			} else {
				// 该节点并不是根节点，加入到父节点中
				ast::AstNode *parent = stack.peek();
				parent->Children()->add(n);
			}

			stack.push(n);
		}

		// 解析完后，判断栈内是否还有节点，如有，则代表结束单元缺失
		if (!stack.empty()) {
			THROW(exception::astfilereader::EndNodeError("read()"));
			return root;
		}

		return root;
	}
};
} // namespace stamon::action

#undef CE