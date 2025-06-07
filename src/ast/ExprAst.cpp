/*
	Name: ExprAst.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 28/07/23 15:48
	Description: 在这里定义与表达式有关的Ast节点
*/

#pragma once

#include "Ast.hpp"

namespace stamon::ast {
enum _PostfixType {
	// 后缀类型定义
	PostfixMemberType = 0, // 成员后缀
	PostfixElementType, // 下标后缀
	PostfixCallType, // 调用函数后缀
	PostfixNewType // 新建对象后缀
};
enum _BinaryOperatorType {
	// 双目运算符类型定义
	BinaryLogicORType = 0, // 逻辑或
	BinaryLogicANDType, // 逻辑与
	BinaryBitORType, // 按位或
	BinaryBitXORType, // 按位异或
	BinaryBitANDType, // 按位与
	BinaryEqualityType, // 是否相等
	BinaryInequalityType, // 是否不相等
	BinaryBigThanType, // 大于
	BinaryLessThanType, // 小于
	BinaryBigThanOrEqualType, // 大等于
	BinaryLessThanOrEqualType, // 小等于
	BinaryLeftShiftType, // 左移位
	BinaryRightShiftType, // 右移位
	BinaryAddType, // 加法
	BinarySubType, // 减法
	BinaryMultType, // 乘法
	BinaryDiviType, // 除法
	BinaryModType, // 取余
	BinaryTypeCount // 类型总数
};
enum _UnaryOperatorType {
	// 单目运算符类型定义
	UnaryPositiveType = 0, // 正
	UnaryNegativeType, // 负
	UnaryNotType, // 非
	UnaryInverseType, // 按位反
	UnaryTypeCount // 类型总数
};

class AstExpression : public AstNode {
public:
	int ass_type;

	AstExpression()
		: AstNode(AstExpressionType) {
	}

	AstExpression(AstLeftValue *LeftValue, int AssTok, AstExpression *expr)
		: AstNode(AstExpressionType) {
		children->add((AstNode *) LeftValue);
		children->add((AstNode *) expr);
		ass_type = AssTok;
	}
	AstExpression(AstBinary *value)
		: AstNode(AstExpressionType) {
		children->add((AstNode *) value);
		ass_type = -1;
	}
	virtual ~AstExpression() = default;
};
class AstLeftValue : public AstNode {
public:
	AstLeftValue()
		: AstNode(AstLeftValueType) {
	}

	AstLeftValue(AstIdentifier *iden, ArrayList<AstNode *> *postfix)
		: AstNode(AstLeftValueType) {
		children = postfix;
		children->insert(0, (AstNode *) iden);
	}
};
class AstBinary : public AstNode {
public:
	int operator_type;
	int getOperatorType() {
		return operator_type;
	}

	AstBinary()
		: AstNode(AstBinaryType) {
	}

	AstBinary(int OperatorType, AstNode *left, AstNode *right)
		: AstNode(AstBinaryType) {
		operator_type = OperatorType;
		children->add((AstNode *) left);
		children->add((AstNode *) right);
	}
	AstBinary(AstNode *left)
		: AstNode(AstBinaryType) {
		operator_type = -1;
		children->add((AstNode *) left);
	}
	virtual ~AstBinary() = default;
};
class AstUnary : public AstNode {
public:
	int operator_type;
	int getOperatorType() {
		return operator_type;
	}
	AstUnary()
		: AstNode(AstUnaryType) {
	}
	AstUnary(int OperatorType, AstNode *value)
		: AstNode(AstUnaryType) {
		operator_type = OperatorType;
		children->add((AstNode *) value);
	}
	AstUnary(AstNode *value, ArrayList<AstNode *> *postfix)
		: AstNode(AstUnaryType) {
		operator_type = -1;
		children = postfix;
		children->insert(0, (AstNode *) value);
	}
	virtual ~AstUnary() = default;
};
class AstPostfix : public AstNode {
public:
	int postfix_type;
	int getPostfixType() {
		return postfix_type;
	}
	AstPostfix()
		: AstNode(AstPostfixType) {
	}
	AstPostfix(int PostfixType, AstNode *value)
		: AstNode(AstPostfixType) {
		postfix_type = PostfixType;
		children->add((AstNode *) value);
	}
	virtual ~AstPostfix() = default;
};
class AstArguments : public AstNode {
public:
	AstArguments()
		: AstNode(AstArgumentsType) {
	}

	AstArguments(ArrayList<AstNode *> *exprs)
		: AstNode(AstArgumentsType) {
		children = exprs;
	}
};
class AstArrayLiteral : public AstNode {
public:
	AstArrayLiteral()
		: AstNode(AstArrayLiteralType) {
	}

	AstArrayLiteral(AstExpression *expr)
		: AstNode(AstArrayLiteralType) {
		children->add((AstNode *) expr);
	}
};
class AstListLiteral : public AstNode {
public:
	AstListLiteral()
		: AstNode(AstListLiteralType) {
	}

	AstListLiteral(ArrayList<AstNode *> *exprs)
		: AstNode(AstListLiteralType) {
		children = exprs;
	}
};

} // namespace stamon::ast