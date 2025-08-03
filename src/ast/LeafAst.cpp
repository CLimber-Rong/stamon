/*
	Name: LeafAst.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 29/07/23 20:14
	Description: 语法树叶子节点的定义
*/

#pragma once

#include "Ast.hpp"
#include "String.hpp"

namespace stamon::ast {
class AstIdentifier : public AstNode {
	unsigned long long id;

public:
	AstIdentifier(unsigned long long iden_id)
		: AstNode(AstIdentifierType) {
		id = iden_id;
	}
	unsigned long long getID() {
		return id;
	}
	virtual ~AstIdentifier() = default;
};
/*
 * 这里需要解释一下
 * 由于虚拟机的设计，需要定义两种标识符节点
 * AstIdentifierName存储着标识符的名字（字符串）
 * 这个节点并不参与虚拟机的运行，而是参与目标代码的编译
 * 如果你是来阅读虚拟机实现代码的，请不要理会这个节点
 * AstIdentifier存储着标识符的编号，这个节点用作运行
 */
class AstIdentifierName : public AstIdentifier {
	String name;

public:
	AstIdentifierName(const String &id)
		: AstIdentifier(-1) {
		name = id;
	}
	String getName() {
		return name;
	}
	virtual ~AstIdentifierName() = default;
};

/*
 * 在 虚拟机AST设计.md 里，你会看到一个名为“NUMBER”的节点
 * 由于数字字面量有很多种（例如整数、浮点数）
 * 所以我先编写了AstNumber作为数字字面量的基类，所有的数字字面量节点都继承这个基类
 */

enum _NumberType { IntNumberType, FloatNumberType, DoubleNumberType };

class AstNumber : public AstNode {
protected:
	int number_type;

public:
	AstNumber()
		: AstNode(AstNumberType) {
	}
	int getNumberType() {
		return number_type;
	}
	virtual ~AstNumber() = default;
};

class AstIntNumber : public AstNumber {
	int value;

public:
	AstIntNumber(int val)
		: AstNumber() {
		number_type = IntNumberType;
		value = val;
	}
	int getVal() {
		return value;
	}
	virtual ~AstIntNumber() = default;
};

class AstFloatNumber : public AstNumber {
	float value;

public:
	AstFloatNumber(float val)
		: AstNumber() {
		number_type = FloatNumberType;
		value = val;
	}
	float getVal() {
		return value;
	}
	virtual ~AstFloatNumber() = default;
};

class AstDoubleNumber : public AstNumber {
	double value;

public:
	AstDoubleNumber(double val)
		: AstNumber() {
		number_type = DoubleNumberType;
		value = val;
	}
	double getVal() {
		return value;
	}
	virtual ~AstDoubleNumber() = default;
};

class AstString : public AstNode {
	String val;

public:
	AstString(const String &str)
		: AstNode(AstStringType) {
		val = str;
	}
	String getVal() {
		return val;
	}
	virtual ~AstString() = default;
};

class AstNull : public AstNode {
public:
	AstNull()
		: AstNode(AstNullType) {
	}
};

class AstBreak : public AstNode {
public:
	AstBreak()
		: AstNode(AstBreakType) {
	}
};

class AstContinue : public AstNode {
public:
	AstContinue()
		: AstNode(AstContinueType) {
	}
};

class AstLeaf : public ast::AstNode {
public:
	int index; // 常量表下标
	AstLeaf()
		: AstNode(ast::AstLeafType) {
	}
	AstLeaf(int data)
		: AstNode(ast::AstLeafType) {
		index = data;
	}
	virtual int getVal() const {
		return index;
	}
	virtual ~AstLeaf() = default;
};
} // namespace stamon::ast