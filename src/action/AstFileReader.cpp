/*
	Name: AstFileReader.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 25/04/25 18:55
	Description: AST编码读取器
*/

#pragma once

#include "Ast.hpp"
#include "BinaryReader.hpp"
#include "Exception.hpp"
#include "Stack.hpp"
#include "String.hpp"

#define CE \
	CATCH { \
		return NULL; \
	}

#define CHECK_AST(ast_type) \
	if (type == ast_type##Type) { \
		n = new ast_type(); \
	}

#define CHECK_SPECIAL_AST(ast_type, special_member) \
	if (type == ast_type##Type) { \
		int data = (readbyte() << 24) + (readbyte() << 16) + (readbyte() << 8) \
				 + (readbyte()); \
		n n = new ast_type(); \
		n->special_member = data; \
	}

namespace stamon::action {
class AstFileReader {
	char *buffer; // 二进制码
	int buffer_size; // 二进制文件大小
	int pos; // 设当前需要读取的字节为buffer[pos]
	BinaryReader reader;

public:
	STMException *ex;

	AstReader() {
	}

	AstReader(STMException *e, String filename) {
		ex = e;
		reader = BinaryReader(e, filename);
		CATCH {
			return;
		}

		buffer = reader.read(); // 读取
		pos = 0;
		buffer_size = reader.getsize();

		if (buffer_size < 2 || buffer[0] != 0xAB || buffer[1] != 0xDC) {
			// 如果文件过小或魔数异常则报错
			THROW_S(String("non-standardized ast-file"));
			return;
		}
		buffer += 2; // 从buffer[2]开始读
	}

	char readbyte() {
		if (pos >= buffer_size) {
			THROW_S(String("non-standardized ast-file"));
			return 0;
		}
		char rst = buffer[pos];
		pos++;
		return rst;
	}

	int updateLineNo() {
		// 更新行号
		int rst = (readbyte() << 24) + (readbyte() << 16) + (readbyte() << 8)
				+ (readbyte());
		CATCH {
			return -1;
		}
		return rst;
	}

	String updateFileName() {
		int slen = (readbyte() << 24) + (readbyte() << 16) + (readbyte() << 8)
				 + (readbyte());

		CATCH {
			return String();
		}

		char *c_arr = new char[slen];

		for (int i = 0; i < slen; i++) {
			c_arr[i] = buffer[pos + i];
		}

		String filename = String(c_arr);

		delete[] c_arr;

		return filename;
	}

	AstNode *readNode(int type) {
		ast::AstNode *n = NULL;
		CHECK_AST(ast::AstProgram);
		CHECK_AST(ast::AstDefClass);
		CHECK_AST(ast::AstDefFunc);
		CHECK_AST(ast::AstDefVar);
		CHECK_AST(ast::AstAnonFunc);
		CHECK_AST(ast::AstBlock);
		CHECK_AST(ast::AstBreak);
		CHECK_AST(ast::AstContinue);
		CHECK_AST(ast::AstIfStatement);
		CHECK_AST(ast::AstWhileStatement);
		CHECK_AST(ast::AstForStatement);
		CHECK_AST(ast::AstReturnStatement);
		CHECK_AST(ast::AstSFN);
		CHECK_AST(ast::AstLeftValue);
		CHECK_AST(ast::AstArguments);
		CHECK_AST(ast::AstNull);
		CHECK_AST(ast::AstArrayLiteral);
		CHECK_AST(ast::AstListLiteral);

		// 对有ast数据的节点进行特判
		if (ast::AstIdentifierType) {
			// 标识符
			int len = (readbyte() << 24) + (readbyte() << 16) + (readbyte() << 8)
					+ (readbyte());
			CE;
			// 再读取到char数组里
			char *cstr = new char[len + 1];
			for (int i = 0; i < len; i++) {
				cstr[i] = readbyte();
			}
			CE;
			String iden(cstr, len);
			delete cstr; // 释放内存
			n = new ast::AstIdentifierName(iden);
		}

		if (ast::AstNumberType) {
			// 数字
			int numtype = readbyte();
			CE;

			switch (numtype) {
			case ast::IntNumberType: {
				// 整数
				int val = (readbyte() << 24) + (readbyte() << 16)
						+ (readbyte() << 8) + (readbyte());
				CE;
				n = new ast::AstIntNumber(val);
				break;
			}
			case ast::FloatNumberType: {
				// 单精度浮点
				float val;

				char *ptr = (char *) &val;

				ptr[0] = readbyte();
				ptr[1] = readbyte();
				ptr[2] = readbyte();
				ptr[3] = readbyte();

				CE;
				n = new ast::AstFloatNumber(val);
				break;
			}
			case ast::DoubleNumberType: {
				// 双精度浮点
				double val;

				char *ptr = (char *) &val;

				ptr[0] = readbyte();
				ptr[1] = readbyte();
				ptr[2] = readbyte();
				ptr[3] = readbyte();
				ptr[4] = readbyte();
				ptr[5] = readbyte();
				ptr[6] = readbyte();
				ptr[7] = readbyte();

				CE;
				n = new ast::AstDoubleNumber(val);
				break;
			}
			}
		}

		if (ast::AstStringType) {
			// 字符串
			// 先读取出字符串长度
			int len = (readbyte() << 24) + (readbyte() << 16) + (readbyte() << 8)
					+ (readbyte());
			CE;
			// 再读取到char数组里
			char *cstr = new char[len + 1];
			for (int i = 0; i < len; i++) {
				cstr[i] = readbyte();
			}
			CE;
			String val(cstr, len);
			delete cstr; // 释放内存
			n = new ast::AstString(val);
		}

		if (ast::AstAnonClassType) {
			// 匿名类
			char isHaveFather = readbyte();
			CE;

			n = new ast::AstAnonClass();
			((ast::AstAnonClass *) n)->isHaveFather = isHaveFather;
		}

		CHECK_SPECIAL_AST(ast::AstExpression, ass_type);
		CHECK_SPECIAL_AST(ast::AstLeftPostfix, postfix_type);
		CHECK_SPECIAL_AST(ast::AstBinary, operator_type);
		CHECK_SPECIAL_AST(ast::AstUnary, operator_type);
		CHECK_SPECIAL_AST(ast::AstPostfix, postfix_type);
	}

	ast::AstNode *read() {
		ArrayList<ast::AstNode *> flatAstNode;
		// 平面的节点（包括结尾单元）

		int lineno = 0;
		String filename;

		while (pos < buffer_size) {
			int type = readbyte();
			CE;

			while (type == -2 || type == -3) {
				if (type == -2) {
					lineno = updateLineNo();
				}
				if (type == -3) {
					filename = updateFileName();
				}
				type = readbyte();
				CE;
			}

			ast::AstNode *node = readNode();
			node->lineNo = lineno;
			node->filename = filename;

			flatAstNode.add(node);
		}

		Stack<ast::AstNode> stack;
		for (int i = 0; i < flatAstNode.size(); i++) {
			ast::AstNode *node = flatAstNode[i];
			if (node->getType() == -1) {
				stack.pop();
			} else {
				stack.peek()->Children->add(node);
			}
		}

		ast::AstNode *rst = stack.pop();

		return rst;
	}
};
} // namespace stamon::action

#undef CHECK_AST
#undef CHECK_SPECIAL_AST
#undef CE