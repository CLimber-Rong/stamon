/*
	Name: AstIr.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 09/02/24 08:48
	Description: ast::Ast-IR生成器
*/

#pragma once

#include "ArrayList.hpp"
#include "Ast.hpp"
#include "ByteMap.hpp"
#include "DataType.hpp"
#include "Exception.hpp"
#include "NumberMap.hpp"
#include "Stack.hpp"
#include "String.hpp"
#include "StringMap.hpp"

// 为了方便，我定义了宏
// 这些宏只能在本文件中使用

#define CHECK_SPECIAL_AST(type, ast_data) \
	if (top->getType() == type##Type) { \
		rst.data = ((type *) top)->ast_data; \
	}

#define CHECK_IR(ast_type) \
	if (ir[i].type == ast_type##Type) { \
		n = new ast_type(); \
		n->lineNo = ir[i].lineNo; \
		n->filename = ir[i].filename; \
	}

#define CHECK_SPECIAL_IR(ast_type, ast_data) \
	if (ir[i].type == ast_type##Type) { \
		n = (ast::AstNode *) new ast_type(); \
		((ast_type *) n)->ast_data = ir[i].data; \
	}

namespace stamon::ir {

class AstIr {
public:
	int type;
	/*
	 * 一些特别的规定
	 * 当type为-1，代表这是一个结束符，即</>
	 */
	int data;
	// 如果这个IR是字面量或标识符，则data存储着其在常量表中的下标
	// 否则存储这个IR的具体信息（例如运算符类型）
	String filename; // IR所在的文件名
	int lineNo; // IR所在的行号
};

enum { IdenConstTypeID = -1 };

class IdenConstType : public datatype::DataType {
	// 我需要在生成常量表时将标识符和数据类型统一
	// 所以我建立了IdenConstType
	String iden;

public:
	IdenConstType(const String &value)
		: datatype::DataType(-1) {
		iden = value;
	}
	virtual String getVal() const {
		return iden;
	}
};

class AstIrConverter {
public:
	ArrayList<datatype::DataType *> tableConst;
	ByteMap<void> tableConstFloat;
	ByteMap<void> tableConstDouble;
	NumberMap<void> tableConstInt;
	StringMap<void> tableConstString;
	StringMap<void> tableConstIden;

	STMException *ex;

	/*
	 * 这里需要解释一下
	 * 以上是不同类型的常量表
	 * 无论是ByteMap，抑或是NumberMap、StringMap
	 * 如果设置模板类型为void，即代表该map的值的类型为void*（详见库的定义）
	 * 这些void*里存的是整数，即键在tableConst当中的下标
	 * 举个例子：
	 * 遇到整数常量1145
	 * 以1145为键，在tableConstInt中查找值
	 * 假设查到值为14
	 * 则代表tableConst[14]是一个IntegerType，且值为1145
	 */

	AstIrConverter() {
	}
	AstIrConverter(STMException *e) {
		ex = e;
	}

	int createConst(datatype::DataType *dt) {
		/*
		 * 新建一个常量，常量的值为dt
		 * 在调用这个函数前，请确保dt不在常量表中
		 * 返回新建后，该常量在常量表中的下标
		 */
		tableConst.add(dt);

		int index = tableConst.size() - 1;

		if (dt->getType() == datatype::IntegerTypeID) {
			datatype::IntegerType *dtInt = (datatype::IntegerType *) dt;
			tableConstInt.put(dtInt->getVal(), *(void **) &index);
			//*(void**)&index等价于(void*)index
		}

		if (dt->getType() == datatype::FloatTypeID) {
			datatype::FloatType *dtFlt = (datatype::FloatType *) dt;

			float val = dtFlt->getVal();
			char *key = (char *) &val;

			tableConstFloat.put(key, sizeof(float), *(void **) &index);
		}

		if (dt->getType() == datatype::DoubleTypeID) {
			datatype::DoubleType *dtDb = (datatype::DoubleType *) dt;

			double val = dtDb->getVal();
			char *key = (char *) &val;

			tableConstFloat.put(key, sizeof(double), *(void **) &index);
		}

		if (dt->getType() == datatype::StringTypeID) {
			datatype::StringType *dtStr = (datatype::StringType *) dt;

			String key = dtStr->getVal();

			tableConstString.put(key, *(void **) &index);
		}

		if (dt->getType() == -1) {
			// 新建的是标识符
			IdenConstType *dtIden = (IdenConstType *) dt;

			String key = dtIden->getVal();

			tableConstIden.put(key, *(void **) &index);
		}

		return index;
	}

	int searchConst(datatype::DataType *dt) {
		// 查找dt在常量表当中的下标
		// 如果dt不在常量表中，则返回-1

		if (dt->getType() == datatype::IntegerTypeID) {
			datatype::IntegerType *dtInt = (datatype::IntegerType *) dt;
			if (!tableConstInt.containsKey(dtInt->getVal())) {
				// 不在常量表中
				return -1;
			}
			return (long long) tableConstInt.get(dtInt->getVal());
		}

		if (dt->getType() == datatype::FloatTypeID) {
			datatype::FloatType *dtFlt = (datatype::FloatType *) dt;

			float val = dtFlt->getVal();
			char *key = (char *) &val;

			if (!tableConstFloat.containsKey(key, sizeof(float))) {
				return -1;
			}
			return (long long) tableConstFloat.get(key, sizeof(float));
		}

		if (dt->getType() == datatype::DoubleTypeID) {
			datatype::DoubleType *dtDb = (datatype::DoubleType *) dt;

			double val = dtDb->getVal();
			char *key = (char *) &val;

			if (!tableConstDouble.containsKey(key, sizeof(double))) {
				return -1;
			}
			return (long long) tableConstDouble.get(key, sizeof(double));
		}

		if (dt->getType() == datatype::StringTypeID) {
			datatype::StringType *dtStr = (datatype::StringType *) dt;

			String key = dtStr->getVal();

			if (!tableConstString.containsKey(key)) {
				return -1;
			}
			return (long long) tableConstString.get(key);
		}

		if (dt->getType() == -1) {
			// 查找的是标识符
			IdenConstType *dtIden = (IdenConstType *) dt;

			String key = dtIden->getVal();

			if (!tableConstIden.containsKey(key)) {
				return -1;
			}
			return (long long) tableConstIden.get(key);
		}

		return -1;
	}

	ArrayList<AstIr> ast2ir(ast::AstNode *program) {
		Stack<ast::AstNode> stack;
		ArrayList<AstIr> list;

		if (program == NULL) {
			return list;
		}

		// 先把__init__放入常量表
		// 这也就意味着__init__始终在常量表的第一位
		createConst(new IdenConstType(String((char *) "__init__")));

		stack.push(program);

		// 迭代遍历语法树，编译成AstIr

		while (stack.empty() == false) {
			bool isLeafNode = false;
			AstIr rst;
			ast::AstNode *top = stack.pop(); // 弹出栈顶

			rst.type = top->getType();
			rst.data = 0; // 默认为0
			rst.lineNo = top->lineNo;
			rst.filename = top->filename;

			// 先特判一些节点
			CHECK_SPECIAL_AST(ast::AstAnonClass, isHaveFather)
			CHECK_SPECIAL_AST(ast::AstExpression, ass_type)
			CHECK_SPECIAL_AST(ast::AstBinary, getOperatorType())
			CHECK_SPECIAL_AST(ast::AstUnary, getOperatorType())
			CHECK_SPECIAL_AST(ast::AstPostfix, getPostfixType())
			// 特判结束符
			if (top->getType() == ast::AstNodeType) {
				isLeafNode = true;
				rst.type = -1;
				rst.lineNo = -1;
				rst.filename = String((char *) "");
			}
			// 特判叶子节点

			if (top->getType() == ast::AstNumberType
					|| top->getType() == ast::AstStringType
					|| top->getType() == ast::AstIdentifierType) {
				isLeafNode = true;
				rst.type = ast::AstLeafType; // 统一代表叶子节点
			}

			if (top->getType() == ast::AstIdentifierType) {
				String iden = ((ast::AstIdentifierName *) top)->getName();
				IdenConstType *dtIden = new IdenConstType(iden);

				int index; // 常量表下标

				index = searchConst(dtIden);

				if (index == -1) {
					index = createConst(dtIden);
				}

				rst.data = index;
			}

			if (top->getType() == ast::AstStringType) {
				String s = ((ast::AstString *) top)->getVal();
				datatype::StringType *dt = new datatype::StringType(s);

				int index; // 常量表下标

				index = searchConst(dt);

				if (index == -1) {
					index = createConst(dt);
				}

				rst.data = index;
			}

			if (top->getType() == ast::AstNumberType) {
				int num_type = ((ast::AstNumber *) top)->getNumberType();
				if (num_type == ast::IntNumberType) {
					int n = ((ast::AstIntNumber *) top)->getVal();
					datatype::IntegerType *dt = new datatype::IntegerType(n);

					int index; // 常量表下标

					index = searchConst(dt);

					if (index == -1) {
						index = createConst(dt);
					}

					rst.data = index;
				}
				if (num_type == ast::FloatNumberType) {
					float n = ((ast::AstFloatNumber *) top)->getVal();
					datatype::FloatType *dt = new datatype::FloatType(n);

					int index; // 常量表下标

					index = searchConst(dt);

					if (index == -1) {
						index = createConst(dt);
					}

					rst.data = index;
				}
				if (num_type == ast::DoubleNumberType) {
					double n = ((ast::AstDoubleNumber *) top)->getVal();
					datatype::DoubleType *dt = new datatype::DoubleType(n);

					int index; // 常量表下标

					index = searchConst(dt);

					if (index == -1) {
						index = createConst(dt);
					}

					rst.data = index;
				}
			}

			// 将AST IR存入列表

			list.add(rst);

			if (isLeafNode == false) {
				// 压入终结符
				ast::AstNode *end = new ast::AstNode();
				end->lineNo = rst.lineNo;

				stack.push(end);

				// 接着遍历子节点
				for (int i = top->Children()->size() - 1; i >= 0; i--) {
					stack.push(top->Children()->at(i));
				}
			}
		}

		return list;
	}

	ast::AstNode *ir2ast(ArrayList<AstIr> ir) {
		/*
		 * AstIr转Ast
		 * 如果ir不正确，程序会运行时错误
		 * 所以请在运行该函数前检查ir
		 */

		Stack<ast::AstNode> stack;

		ast::AstNode *root = NULL;

		if (ir.size() == 0) {
			THROW_S(String("expect ast-ir root node"));
			return root;
		}

		for (int i = 0, len = ir.size(); i < len; i++) {
			if (ir[i].type == -1) {
				// 特判结束符

				if (stack.empty()) {
					// 多余的终结符
					THROW_S(String("redundant ast-ir end node"));
					return root;
				}

				stack.pop();
				continue;
			}

			// 创建一个节点
			ast::AstNode *n = NULL;

			CHECK_SPECIAL_IR(ast::AstLeaf, index);
			CHECK_SPECIAL_IR(ast::AstAnonClass, isHaveFather);
			CHECK_SPECIAL_IR(ast::AstExpression, ass_type);
			CHECK_SPECIAL_IR(ast::AstBinary, operator_type);
			CHECK_SPECIAL_IR(ast::AstUnary, operator_type);
			CHECK_SPECIAL_IR(ast::AstPostfix, postfix_type);

			if (n == NULL) {
				// 非特殊节点
				n = new ast::AstNode();
				n->id = ir[i].type;
			}

			n->lineNo = ir[i].lineNo;
			n->filename = ir[i].filename;

			if (stack.empty()) {
				// 该节点是根节点

				if (root != NULL) {
					// 已经出现根节点了，因此是重复的根节点
					THROW_S(String("redundant ast-ir root node"));
					return root;
				}

				root = n;

			} else {
				// 该节点并不是根节点，加入到父节点中
				ast::AstNode *parent = stack.peek();
				parent->Children()->add(n);
			}

			if(ir[i].type!=ast::AstLeafType) {
				//叶子节点没有对应的结束单元，不需要压入
				stack.push(n);
			}

		}

		// 解析完后，判断栈内是否还有节点，如有，则代表结束单元缺失
		if (stack.empty() == false) {
			THROW_S(String("expect ast-ir end node"));
			return root;
		}

		return root;
	}

	void destroyConst(ArrayList<datatype::DataType *> tabconst) {
		// 销毁指定的常量表
		for (int i = 0; i < tabconst.size(); i++) {
			delete tabconst[i];
		}
	}
};
} // namespace stamon::ir

#undef CHECK_SPECIAL_AST
#undef CHECK_IR
#undef CHECK_SPECIAL_IR