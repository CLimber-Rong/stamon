/*
	Name: AstFileWriter.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 25/04/25 18:56
	Description: AST编码文件写入器
*/

#pragma once

#include "Ast.hpp"
#include "BinaryWriter.hpp"
#include "Exception.hpp"
#include "Stack.hpp"

// 为了节约篇幅，定义了一些宏用于简写
// 这些宏只用于此文件

#define WRITE(b) \
	writer.write(b); \
	CATCH { \
		return; \
	}

#define WRITE_I(n) \
	writer.write_i(n); \
	CATCH { \
		return; \
	}

namespace stamon::action {

class AstFileWriter {
	BinaryWriter writer;

public:
	STMException *ex;

	AstFileWriter() {
	}

	AstFileWriter(STMException *e, String filename) {
		ex = e;
		writer = BinaryWriter(ex, filename);

		CATCH {
			return;
		}

		WRITE(0xAB);
		WRITE(0xDD);
	}

	void write(ast::AstNode *node, bool isstrip) {
		String filename;
		int lineno = -1;

		Stack<ast::AstNode> stack;
		stack.push(node);

		while (stack.empty() == false) {
			ast::AstNode *top = stack.pop();

			if(top==NULL) {
				//结束符
				WRITE(-1);
				continue;
			}

			if (isstrip == false) {
				// 需要输出调试信息

				if (top->lineNo != lineno) {
					WRITE(-2);
					lineno = top->lineNo;
					WRITE_I(lineno);
				}

				if (top->filename != filename) {
					// 输出文件信息
					WRITE(-3);

					filename = top->filename;

					WRITE_I(filename.length()); // 写入长度

					for (int i = 0; i < filename.length(); i++) {
						WRITE(filename[i]);
					}
				}
			}

			writeNode(top);

			stack.push(NULL);	//压入结束符

			for(int i=top->Children()->size()-1; i>=0; i--) {
				stack.push(top->Children()->at(i));
			
			}

			CATCH {
				return;
			}
		}
	}

	void writeNode(ast::AstNode *top) {
		WRITE(top->getType());

		switch (top->getType()) {
		// 对有ast数据的节点进行特判
		case ast::AstIdentifierType: {
			// 标识符
			String iden = ((ast::AstIdentifierName *) top)->getName();
			WRITE_I(iden.length());
			for (int i = 0; i < iden.length(); i++) {
				WRITE(iden[i]);
			}
			break;
		}

		case ast::AstNumberType: {
			// 数字
			ast::AstNumber *number = (ast::AstNumber *) top;

			WRITE(number->getNumberType());

			switch (number->getNumberType()) {
			case ast::IntNumberType: {
				// 整数
				int val = ((ast::AstIntNumber *) number)->getVal();
				WRITE_I(val);
				break;
			}
			case ast::FloatNumberType: {
				// 单精度浮点
				float val = ((ast::AstFloatNumber *) number)->getVal();
				// 逐个字节写入
				char *val_ptr = (char *) &val;
				WRITE(val_ptr[0]);
				WRITE(val_ptr[1]);
				WRITE(val_ptr[2]);
				WRITE(val_ptr[3]);
				break;
			}
			case ast::DoubleNumberType: {
				// 双精度浮点
				double val = ((ast::AstDoubleNumber *) number)->getVal();
				// 逐个字节写入
				char *val_ptr = (char *) &val;
				WRITE(val_ptr[0]);
				WRITE(val_ptr[1]);
				WRITE(val_ptr[2]);
				WRITE(val_ptr[3]);
				WRITE(val_ptr[4]);
				WRITE(val_ptr[5]);
				WRITE(val_ptr[6]);
				WRITE(val_ptr[7]);
				break;
			}
			}
			break;
		}

		case ast::AstStringType: {
			// 字符串
			String text = ((ast::AstString *) top)->getVal();

			WRITE_I(text.length());
			for (int i = 0; i < text.length(); i++) {
				WRITE(text[i]);
			}
			break;
		}

		case ast::AstAnonClassType: {
			// 匿名类
			WRITE(((ast::AstAnonClass *) top)->isHaveFather);
			break;
		}

		case ast::AstExpressionType: {
			// 匿名类
			WRITE_I(((ast::AstExpression *) top)->ass_type);
			break;
		}

		case ast::AstBinaryType: {
			// 匿名类
			WRITE_I(((ast::AstBinary *) top)->operator_type);
			break;
		}

		case ast::AstUnaryType: {
			// 匿名类
			WRITE_I(((ast::AstUnary *) top)->operator_type);
			break;
		}

		case ast::AstPostfixType: {
			// 匿名类
			WRITE_I(((ast::AstPostfix *) top)->postfix_type);
			break;
		}
		}
	}

	void close() {
		writer.close();
	}
};

} // namespace stamon::action

#undef WRITE
#undef WRITE_I