// AstLeftValue* left_value() {
				// 	AstIdentifierName* iden = IDEN();
				// 	CE
				// 	ArrayList<AstNode*>* postfixs = new ArrayList<AstNode*>();
				// 	while(true) {
				// 		//只要还有后缀，就不停匹配
				// 		if(check(TokenMember)) {
				// 			//成员后缀
				// 			_pop;
				// 			IdenToken* tok = (IdenToken*)match(TokenIden);
				// 			CTH("the member name must be an identifier")
				// 			AstIdentifierName* member = Ast<AstIdentifierName>(
				// 			                                iden->lineNo,
				// 			                                tok->iden
				// 			                            );
				// 			AstLeftPostfix* p = Ast<AstLeftPostfix>(
				// 			                        member->lineNo,
				// 			                        PostfixMemberType,
				// 			                        member
				// 			                    );
				// 			postfixs->add(p);
				// 		} else if(check(TokenLSB)) {
				// 			//下标后缀
				// 			_pop;
				// 			AstExpression* expr = expression();
				// 			CE
				// 			match(TokenRSB);
				// 			CTH("the square parentheses are not closed")
				// 			AstLeftPostfix* p = Ast<AstLeftPostfix>(
				// 			                        expr->lineNo,
				// 			                        PostfixElementType,
				// 			                        expr
				// 			                    );
				// 			postfixs->add(p);
				// 		} else {
				// 			break;	//没有后缀，退出
				// 		}
				// 	}
				// 	return Ast<AstLeftValue>(iden->lineNo, iden, postfixs);
				// }