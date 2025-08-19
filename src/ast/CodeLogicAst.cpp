/*
	Name: CodeLogicAst.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 28/07/23 15:45
	Description: 在这里写代码逻辑（如函数、流程控制等）节点的定义
*/

#pragma once

#include "Ast.hpp"

namespace stamon::ast {
class AstProgram : public AstNode {
public:
	AstProgram()
		: AstNode(AstProgramType) {
	}

	AstProgram(ArrayList<AstNode *> *statements)
		: AstNode(AstProgramType) {
		children = statements;
	}
};
class AstDefClass : public AstNode {
public:
	AstDefClass()
		: AstNode(AstDefClassType) {
	}

	AstDefClass(AstIdentifier *iden, AstAnonClass *object_class)
		: AstNode(AstDefClassType) {
		children->add((AstNode *) iden);
		children->add((AstNode *) object_class);
	}
};
class AstDefFunc : public AstNode {
public:
	AstDefFunc()
		: AstNode(AstDefFuncType) {
	}

	AstDefFunc(AstIdentifier *iden, AstAnonFunc *func)
		: AstNode(AstDefFuncType) {
		children->add((AstNode *) iden);
		children->add((AstNode *) func);
	}
};
class AstDefVar : public AstNode {
public:
	AstDefVar()
		: AstNode(AstDefVarType) {
	}

	AstDefVar(AstIdentifier *iden, AstExpression *expr)
		: AstNode(AstDefVarType) {
		children->add((AstNode *) iden);
		children->add((AstNode *) expr);
	}
};
class AstAnonClass : public AstNode {
public:
	bool father_flag = false;

	AstAnonClass()
		: AstNode(AstAnonClassType) {
	}

	AstAnonClass(AstIdentifier *father, ArrayList<AstNode *> *expr)
		: AstNode(AstAnonClassType) {
		children = expr;
		if (father != NULL) {
			father_flag = true;
			children->insert(0, (AstNode *) father);
		}
	}

	virtual ~AstAnonClass() = default;
};
class AstAnonFunc : public AstNode {
public:
	AstAnonFunc()
		: AstNode(AstAnonFuncType) {
	}

	AstAnonFunc(ArrayList<AstNode *> *args, AstBlock *block)
		: AstNode(AstAnonFuncType) {
		children = args;
		children->add((AstNode *) block);
	}
};
class AstBlock : public AstNode {
public:
	AstBlock()
		: AstNode(AstBlockType) {
	}

	AstBlock(ArrayList<AstNode *> *statements)
		: AstNode(AstBlockType) {
		children = statements;
	}
};
class AstIfStatement : public AstNode {
public:
	AstIfStatement()
		: AstNode(AstIfStatementType) {
	}

	AstIfStatement(AstExpression *expr, AstBlock *block_if)
		: AstNode(AstIfStatementType) {
		children->add((AstNode *) expr);
		children->add((AstNode *) block_if);
	}
	AstIfStatement(AstExpression *expr, AstBlock *block_if, AstBlock *block_else)
		: AstNode(AstIfStatementType) {
		children->add((AstNode *) expr);
		children->add((AstNode *) block_if);
		children->add((AstNode *) block_else);
	}
};
class AstWhileStatement : public AstNode {
public:
	AstWhileStatement()
		: AstNode(AstWhileStatementType) {
	}

	AstWhileStatement(AstExpression *expr, AstBlock *block_while)
		: AstNode(AstWhileStatementType) {
		children->add((AstNode *) expr);
		children->add((AstNode *) block_while);
	}
};
class AstForStatement : public AstNode {
public:
	AstForStatement()
		: AstNode(AstForStatementType) {
	}

	AstForStatement(AstIdentifier *iden, AstExpression *expr, AstBlock *block_for)
		: AstNode(AstForStatementType) {
		children->add((AstNode *) iden);
		children->add((AstNode *) expr);
		children->add((AstNode *) block_for);
	}
};
class AstReturnStatement : public AstNode {
public:
	AstReturnStatement()
		: AstNode(AstReturnStatementType) {
	}

	AstReturnStatement(AstExpression *expr)
		: AstNode(AstReturnStatementType) {
		children->add((AstNode *) expr);
	}
};
} // namespace stamon::ast