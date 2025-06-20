/*
        Name: Ast.hpp
        License: Apache 2.0
        Author: CLimber-Rong
        Date: 28/07/23 16:44
        Description:
        * 这里对Ast进行了基本的定义
        * 如果你想要引用所有的Ast源码，只要写入以下代码即可：
        #include"Ast.hpp"
        using namespace stamon::ast;
*/

#pragma once

#include "ArrayList.hpp"
#include "String.hpp"

namespace stamon {
	namespace ast {
		// Ast类型定义，详细作用见doc/虚拟机AST设计.md
		enum _AstType {
		    AstNodeType = 0,
		    AstProgramType,
		    AstDefClassType,
			AstDefFuncType,
			AstDefVarType,
		    AstAnonClassType,
		    AstAnonFuncType,
		    AstBlockType,
		    AstBreakType,
			AstContinueType,
		    AstIfStatementType,
		    AstWhileStatementType,
		    AstForStatementType,
		    AstReturnStatementType,
		    AstSFNType,
		    AstExpressionType,
		    AstLeftValueType,
		    AstBinaryType,
		    AstUnaryType,
		    AstPostfixType,
			AstArgumentsType,
		    AstIdentifierType,
		    AstNumberType,
		    AstStringType,
		    AstNullType,
		    AstArrayLiteralType,
		    AstListLiteralType,
			AstLeafType,	//用于存储常量表下标的叶子节点
		    AstTypeNum // Ast总数
		};

		// Ast类定义，详细实现见同目录的其他源码
		class AstNode;
		class AstProgram;
		class AstDefClass;
		class AstDefFunc;
		class AstAnonClass;
		class AstAnonFunc;
		class AstBlock;
		class AstBreak;
		class AstContinue;
		class AstIfStatement;
		class AstWhileStatement;
		class AstForStatement;
		class AstReturnStatement;
		class AstSFN;
		class AstExpression;
		class AstLeftValue;
		class AstBinary;
		class AstUnary;
		class AstPostfix;
		class AstArguments;
		class AstIdentifier;
		class AstNumber;
		class AstString;
		class AstNull;
		class AstArrayLiteral;
		class AstListLiteral;
		class AstLeaf;

		class AstNode {
			protected:
				ArrayList<AstNode*>* children;

			public:
				int id;				//语法树的节点类型
				int lineNo;			//语法分析时用于显示行号
				String filename;	//语义分析时用于显示文件名

				AstNode() {
					id = AstNodeType;
					children = new ArrayList<AstNode*>();
				}

				AstNode(int type_id) {
					id = type_id;
					children = new ArrayList<AstNode*>();
				}

				virtual ArrayList<AstNode*> *Children() {
					//获得子节点列表
					return children;
				}

				virtual int getType() {
					//获得节点类型
					return id;
				}

				virtual ~AstNode() {
					//删除该节点及其所有字节点
					for(int i=0;i<children->size();i++) {
						delete children->at(i);
					}
					delete children;
				}
		};
	}
}

#include "CodeLogicAst.cpp"
#include "ExprAst.cpp"
#include "LeafAst.cpp"
#include "SfnAst.cpp"