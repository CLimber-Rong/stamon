/*
	Name: AstFileWriter.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 25/04/25 18:56
	Description: AST编码文件写入器
*/

#pragma once

#include "Ast.hpp"
#include "BufferStream.cpp"
#include "Exception.hpp"
#include "Stack.hpp"

// 为了节约篇幅，定义了一些宏用于简写
// 这些宏只用于此文件

#define CHECK_SPECIAL_AST(ast_type, special_member) \
	case ast_type##Type: { \
		stream.write(((ast_type *) top)->special_member); \
		break; \
	}

namespace stamon::action {

class AstFileWriter {
	BufferOutStream& stream;

public:
	log::Exception *ex;

	AstFileWriter(log::Exception *e, BufferOutStream &outstream)
		: ex(e)
		, stream(outstream) {
		WRITE(0xAB);
		WRITE(0xDD);
	}

	void write(ast::AstNode *node, bool isstrip) {
		String filename;
		int lineno = -1;

		Stack<ast::AstNode*> stack;
		stack.push(node);

		while (!stack.empty()) {
			ast::AstNode *top = stack.pop();

			if (top == NULL) {
				// 结束符
				stream.write((byte) -1);
				continue;
			}

			if (isstrip == false) {
				// 需要输出调试信息

				if (top->lineNo != lineno) {
					stream.write((byte) -2);
					lineno = top->lineNo;
					stream.write(lineno);
				}

				if (top->filename != filename) {
					// 输出文件信息
					stream.write((byte) -3);

					filename = top->filename;

					stream.write(filename.length()); // 写入长度

					for (int i = 0; i < filename.length(); i++) {
						stream.write(filename[i]);
					}
				}
			}

			writeNode(top);

			stack.push(NULL); // 压入结束符

			for (int i = top->Children()->size() - 1; i >= 0; i--) {
				stack.push(top->Children()->at(i));
			}

			CATCH {
				return;
			}
		}
	}

	void writeNode(ast::AstNode *top) {
		stream.write((byte) (top->getType()));

		switch (top->getType()) {
		// 对有ast数据的节点进行特判
		case ast::AstIdentifierType: {
			// 标识符
			String iden = ((ast::AstIdentifierName *) top)->getName();
			stream.write(iden.length());
			for (int i = 0; i < iden.length(); i++) {
				stream.write((byte) (iden[i]));
			}
			break;
		}

		case ast::AstNumberType: {
			// 数字
			ast::AstNumber *number = (ast::AstNumber *) top;

			stream.write((byte) (number->getNumberType()));

			switch (number->getNumberType()) {
			case ast::IntNumberType: {
				// 整数
				int val = ((ast::AstIntNumber *) number)->getVal();
				stream.write(val);
				break;
			}
			case ast::FloatNumberType: {
				// 单精度浮点
				float val = ((ast::AstFloatNumber *) number)->getVal();
				stream.write(val);
				break;
			}
			case ast::DoubleNumberType: {
				// 双精度浮点
				double val = ((ast::AstDoubleNumber *) number)->getVal();
				stream.write(val);
				break;
			}
			}
			break;
		}

		case ast::AstStringType: {
			// 字符串
			String text = ((ast::AstString *) top)->getVal();

			stream.write(text.length());
			for (int i = 0; i < text.length(); i++) {
				stream.write(text[i]);
			}
			break;
		}
		}
		CHECK_SPECIAL_AST(ast::AstAnonClass, father_flag);
		CHECK_SPECIAL_AST(ast::AstExpression, ass_type);
		CHECK_SPECIAL_AST(ast::AstBinary, operator_type);
		CHECK_SPECIAL_AST(ast::AstUnary, operator_type);
		CHECK_SPECIAL_AST(ast::AstPostfix, postfix_type);
	}
};

} // namespace stamon::action

#undef CHECK_SPECIAL_AST