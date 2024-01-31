// AstBinary* binary_operator() {
				// 	/*
				// 	 * 如果双目运算符采用递归分析，那么每次分析至少要递归10层函数
				// 	 * 这太耗时了，所以我决定采用迭代的方法来分析双目运算符
				// 	 * 这里的代码参考了清华大学出版社的《数据结构（C语言）》
				// 	 * 所以代码原理不再赘述。
				// 	 * --------------------
				// 	 * 值得注意的是，接下来的代码将会出现"xxx-37"这样的字眼
				// 	 	* 例如TokenLogOR-37
				// 	 * 这其实是将Token中运算符的类型转为Ast中运算符的类型
				// 	 	* 例如：
				// 		* TokenLogOR-37 = 0 = BinaryLogicORType
				// 		* TokenEqu-37 = 5 = BinaryEqualityType
				// 	*/
				// 	Stack<Token> OPTR;		//操作符栈
				// 	Stack<AstNode> OPND;	//操作数栈
				// 	Token* t;

				// 	Token end(-1, 18);		//终结符

				// 	OPTR.push(&end);		//压入终结符
				// 	t = matcher.Peek(0);

				// 	while(
				// 	    (t->type!=TokEOF.type)
				// 		||(t->type!=end.type)
				// 	    ||(OPTR.peek()->type!=18)
				// 	) {
				// 		if(!(TokenLogOR<=t->type&&t->type<=TokenMod)) {
				// 			//不是运算符则进栈
				// 			if(
				// 			    check(TokenIden)
				// 				||check(TokenInt)||check(TokenDouble)
				// 			    ||check(TokenTrue)||check(TokenFalse)
				// 			    ||check(TokenString)
				// 			    ||check(TokenLBC)||check(TokenLSB)
				// 			) {
				// 				//如果是一个操作数
				// 				OPND.push(quark());
				// 				CE
				// 				t = matcher.Peek(0);
				// 			} else {
				// 				//如果不是，则说明这里是表达式结束的地方
				// 				t = &end;
				// 			}

				// 		} else {
				// 			Token* top = OPTR.peek();
				// 			//栈顶的运算符

				// 			if(bin_prio[top->type-37]<bin_prio[t->type-37]) {
				// 				//栈顶运算符优先级低
				// 				OPTR.push(_pop);
				// 				CE
				// 				t = matcher.Peek(0);
				// 			} else if(
				// 			    (bin_prio[top->type-37]==bin_prio[t->type-37])
				// 			    ||(bin_prio[top->type-37]>bin_prio[t->type-37])
				// 			) {
				// 				//栈顶运算符优先级相等或栈顶运算符优先级大
				// 				//退栈并将两个ast节点结合后的新ast节点入栈
				// 				OPTR.pop();
				// 				AstNode *a, *b;
				// 				b = OPND.pop();
				// 				a = OPND.pop();
				// 				OPND.push(Ast<AstBinary>(
				// 				              top->lineNo,
				// 				              top->type-37, a, b)
				// 				         );
				// 			}
				// 		}
				// 	}

				// 	if(OPTR.size()>1) {
				// 		//如果符号栈内还有
				// 	}

				// 	OPTR.clear();	//释放内存
				// 	OPND.destroy();
				// }