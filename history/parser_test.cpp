/*
	Name:
	Copyright:
	Author:
	Date: 09/09/23 13:54
	Description:
*/

#define JUST_DEBUG

#include<iostream>

#include"ArrayList.hpp"
#include"NumberMap.hpp"
#include"Stack.hpp"
#include"String.hpp"
#include"StringMap.hpp"
#include"DataType.hpp"
#include"ObjectManager.cpp"
#include"Ast.hpp"
#include"STVCReader.cpp"
#include"Lexer.cpp"
#include"Parser.cpp"

using namespace stamon::ir;
using namespace stamon::datatype;
using namespace stamon::c;
using namespace std;

void DebugAST(AstNode* node, int layer) {
	for(int i=1;i<=layer;i++) cout<<'\t';
	cout<<"TYPE: "<<node->getType();

	//特判left_postfix
	if(node->getType()==AstLeftPostfixType) {
		AstLeftPostfix* rst = (AstLeftPostfix*)node;
		cout<<" LEFT-POSTFIX: "<<rst->getPostfixType();
	}

	//特判postfix
	if(node->getType()==AstPostfixType) {
		AstPostfix* rst = (AstPostfix*)node;
		cout<<" POSTFIX: "<<rst->getPostfixType();
	}

	//特判单目运算符
	if(node->getType()==AstUnaryType) {
		AstUnary* rst = (AstUnary*)node;
		cout<<" UNARY: "<<rst->getOperatorType();
	}
	//特判双目运算符
	if(node->getType()==AstBinaryType) {
		AstBinary* rst = (AstBinary*)node;
		cout<<" BINARY: "<<rst->getOperatorType();
	}

	//ast的叶子节点要特判
	if(node->getType()==AstIdentifierType) {
		AstIdentifierName* rst = (AstIdentifierName*)node;
		cout<<" IDEN: "<<rst->getName().getstr();
	}
	if(node->getType()==AstNumberType) {
		AstNumber* rst = (AstNumber*)node;
		if(rst->getNumberType()==IntNumberType) {
			cout<<" INT: "<<((AstIntNumber*)rst)->getVal();
		}
		if(rst->getNumberType()==DoubleNumberType) {
			cout<<" DOUBLE: "<<((AstDoubleNumber*)rst)->getVal();
		}
	}
	if(node->getType()==AstStringType) {
		AstString* rst = (AstString*)node;
		cout<<" STRING: "<<rst->getVal().getstr();
	}
	cout<<endl;
	for(int i=0,len=node->Children()->size();i<len;i++) {
		DebugAST(node->Children()->at(i), layer+1);
	}
	return;
}

int main() {
	//在这里编写调试代码，调试方法见文档

	Lexer lexer;
	int index;

	String text((char*)"def a=114514*[810], b = a;");
	index = lexer.getLineTok(1, text);

	index = lexer.getLineTok(1, text);
	CATCH {
		cout<<"ERROR: AT: "<<1<<":"<<index<<" "<<ERROR<<endl;
		return 0;
	}

    Matcher matcher(lexer);

    Parser parser(matcher);

    AstNode* node = parser.Parse();

	CATCH {
		cout<<"Error: "<<ERROR<<endl;
		return -1;
	}

    DebugAST((AstNode*)node, 0);
    
	return 0;
}