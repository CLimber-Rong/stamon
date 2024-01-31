/*
    Name: CodeLogicAst.cpp
    Copyright: Apache 2.0
    Author: CLimber-Rong
    Date: 28/07/23 15:45
    Description: 在这里写代码逻辑（如函数、流程控制等）节点的定义
*/

#ifndef CODELOGICAST_CPP
#define CODELOGICAST_CPP

#include "Ast.hpp"

namespace stamon {
	namespace ast {
		class AstProgram : public AstNode {

			public:

				AstProgram(ArrayList<AstNode*>* defClass) : AstNode() {
					children = defClass;
				}
				virtual int getType() {
					return AstProgramType;
				}
		};
		class AstDefClass : public AstNode {

			public:

				AstDefClass(AstIdentifier* iden, AstAnonClass* object_class) : AstNode() {
					children->add((AstNode*)iden);
					children->add((AstNode*)object_class);
				}
				virtual int getType() {
					return AstDefClassType;
				}
		};
		class AstDefFunc : public AstNode {

			public:

				AstDefFunc(AstIdentifier* iden, AstAnonFunc* func) : AstNode() {
					children->add((AstNode*)iden);
					children->add((AstNode*)func);
				}
				virtual int getType() {
					return AstDefFuncType;
				}
		};
		class AstAnonClass : public AstNode {

			public:

				AstAnonClass(AstIdentifier* father, ArrayList<AstNode*>* expr) : AstNode() {
					children = expr;
					children->add((AstNode*)father);
				}
				virtual int getType() {
					return AstAnonClassType;
				}
		};
		class AstAnonFunc : public AstNode {

			public:

				AstAnonFunc(ArrayList<AstNode*>* args, AstBlock* block) : AstNode() {
					children = args;
					children->add((AstNode*)block);
				}
				virtual int getType() {
					return AstAnonFuncType;
				}
		};
		class AstBlock : public AstNode {

			public:

				AstBlock(ArrayList<AstNode*>* statements) : AstNode() {
					children = statements;
				}
				virtual int getType() {
					return AstBlockType;
				}
		};
		class AstIfStatement : public AstNode {

			public:

				AstIfStatement(AstExpression* expr, AstBlock* block_if) : AstNode() {
					children->add((AstNode*)expr);
					children->add((AstNode*)block_if);
				}
				AstIfStatement(AstExpression* expr, AstBlock* block_if, AstBlock* block_else) : AstNode() {
					children->add((AstNode*)expr);
					children->add((AstNode*)block_if);
					children->add((AstNode*)block_else);
				}
				virtual int getType() {
					return AstIfStatementType;
				}
		};
		class AstWhileStatement : public AstNode {

			public:

				AstWhileStatement(AstExpression* expr, AstBlock* block_while) : AstNode() {
					children->add((AstNode*)expr);
					children->add((AstNode*)block_while);
				}
				virtual int getType() {
					return AstWhileStatementType;
				}
		};
		class AstForStatement : public AstNode {

			public:

				AstForStatement(AstIdentifier* iden, AstExpression* expr, AstBlock* block_for) : AstNode() {
					children->add((AstNode*)iden);
					children->add((AstNode*)expr);
					children->add((AstNode*)block_for);
				}
				virtual int getType() {
					return AstForStatementType;
				}
		};
		class AstReturnStatement : public AstNode {

			public:

				AstReturnStatement(AstExpression* expr) : AstNode() {
					children->add((AstNode*)expr);
				}
				virtual int getType() {
					return AstReturnStatementType;
				}
		};
	}
}

#endif