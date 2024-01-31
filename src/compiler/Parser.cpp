/*
	Name: Parser.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 05/01/24 21:33
	Description: 语法分析器
*/

#ifndef PARSER_CPP
#define PARSER_CPP

#define check(type) (matcher.Check(type))
#define _pop			(matcher.Pop())
#define CE			CATCH { return NULL; }	//如果执行代码中出现异常，直接返回
#define CTH(message)	CATCH { THROW(message) } //如果执行代码中出现异常，抛出异常

#define unary_check(tok_type, unary_type) \
	if(check(tok_type)) {\
		int line = _pop->lineNo;\
		return Ast<AstUnary>(\
		                     line,\
		                     unary_type,\
		                     unary_operator()\
		                    );\
	}

//为了方便编写语法分析的代码，我设计了这些宏
//这些宏只能在这个文件中被使用

#include"Ast.hpp"
#include"Lexer.cpp"
#include"Stack.hpp"

namespace stamon {
	namespace c {

		using namespace stamon::ast;

		class Matcher {
				//用于在语法分析中查看并匹配token
				Lexer lexer;

			public:
				Matcher() {
					//NOTHING
				}

				Matcher(Lexer lex) {
					lexer = lex;
				}

				bool Check(int type) {
					//用于检查token的类型
					if(lexer.peek(0)->type==type) {
						return true;
					}
					return false;
				}

				Token* Match(int type) {
					//匹配一个类型为type的token，如果不匹配，就直接报错
					if(Check(type)) {
						return lexer.getTok();
					} else {
						THROW("invalid syntax")
						return NULL;
					}
				}

				Token* Peek(int index) {
					//返回第index后个token
					return lexer.peek(index);
				}

				Token* Pop() {
					//直接弹出一个token
					return lexer.getTok();
				}
		};

		class SyntaxScope {	//用于记录每个作用域的变量，防止变量重定义、变量未定义就使用
			public:
				//这个类要区分与vm::ObjectScope
				StringMap<void> scope;

				bool exist(Token* iden) {
					return scope.containsKey(((IdenToken*)iden)->iden);
				}

				void mark(Token* iden) {	//声明一个变量
					if(scope.containsKey(((IdenToken*)iden)->iden)) {
						THROW("variables are declared repeatedly")
						return;
					}
					scope.put(((IdenToken*)iden)->iden, NULL);
				}

				void destroy() {
					scope.destroy();
				}
		};

		class Parser {
				Matcher matcher;
				//定义双目运算符的优先级表
				int bin_prio[19] = {
					1, 2, 3, 4, 5,
					6, 6,
					7, 7, 7, 7,
					8, 8,
					9, 9,
					10, 10, 10,
					-1
					//这个-1用于内部的迭代语法分析，我们称为结束符
				};
				//bin_prio[i]代表_BinaryOperatorType中
				//第i个枚举的优先级数值，优先级数值越大，则越为优先

				//定义每个优先级的运算符有哪些
				int bin_layer[10][2] {
					{TokenLogOR, TokenLogOR},
					{TokenLogAND, TokenLogAND},
					{TokenBitOR, TokenBitOR},
					{TokenBitXOR, TokenBitXOR},
					{TokenBitAND, TokenBitAND},
					{TokenEqu, TokenNotEqu},
					{TokenBig, TokenLessEqu},
					{TokenLSH, TokenRSH},
					{TokenAdd, TokenSub},
					{TokenMul, TokenMod}
				};
				/*
				 * 第i层运算符在bin_layer[i-1][0]~bin_layer[i-1][1]的范围内
				 * 例如TokenBigEqu在第7层，
				 	* 则bin_layer[6][0]<=TokenBigEqu<=bin_layer[6][1]
				 * 会发现：我并没有把每层的所有运算符都罗列出来，而是只表示了范围
				*/

				ArrayList<SyntaxScope> scopes;

			public:

				Parser(Matcher matcher) {
					this->matcher = matcher;
					SyntaxScope global_scope;
					scopes.add(global_scope);	//压入一个空的全局作用域
				}

				template<class T, typename...Types>
				T* Ast(int line, Types&& ...args) {
					//这个代码比较难懂，涉及到形参模板和右值引用
					T* rst = new T(args...);
					rst->lineNo = line;
					return rst;
				}

				Token* match(int TokType) {
					Token* rst = matcher.Match(TokType);
					CE
					return rst;
				}


				AstNode* Parse() {	//未完工
					ArrayList<AstNode*>* stm = new ArrayList<AstNode*>();
					statement(stm);
					CE
					return Ast<AstBlock>(
					           0,
					           stm
					       );
				}

				AstProgram* program() {	//未完工
					return NULL;
				}

				AstSFNName* sfn() {
					match(TokenSFN);
					AstIdentifierName *port, *arg;
					port = IDEN();
					match(TokenCmm);
					arg = IDEN();
					match(TokenSemi);
					return new AstSFNName(port, arg);;
				}

				AstBlock* block() {
					//代码块

					ArrayList<AstNode*>* statements = new ArrayList<AstNode*>();

					//特判是否为单个语句的代码块
					//（即simple_block）
					if(check(TokenColon)) {
						int lineNo = _pop->lineNo;	//弹出冒号
						statement(statements);
						CE
						return Ast<AstBlock>(lineNo, statements);
					}

					//如果不是simple_block，那就是常规的代码块

					int lineNo = match(TokenLBC)->lineNo;	//匹配左花括号

					while(check(TokenRBC)==false) {
						statement(statements);
						CE
					}

					_pop;	//弹出右花括号

					return Ast<AstBlock>(lineNo, statements);

				}

				void* statement(ArrayList<AstNode*>* stm) {
					//读取一条语句，并将解析后的ast加入stm当中
					//这里的返回值类型为void*，纯粹为了方便CE时return NULL;
					if(check(TokenDef)) {
						_pop;	//弹出def

						stm->add(def_var());
						CE

						while(check(TokenCmm)) {
							//如果变量的声明不止一个
							_pop;	//弹出逗号
							stm->add(def_var());
							CE
						}

						match(TokenSemi);	//匹配分号
						CE
					} else if(check(TokenFunc)
					          &&matcher.Peek(1)->type==TokenIden) {
						//定义函数
						stm->add(def_func());
						CE
					}
					return NULL;
				}

				AstExpression* MakeNullExpression(Token* iden) {
					//用于生成一个赋值表达式，将iden赋为空值
					return Ast<AstExpression>(
					           iden->lineNo,
					           Ast<AstLeftValue>(	//左值
					               iden->lineNo,
					               Ast<AstIdentifierName>(
					                   iden->lineNo,
					                   ((IdenToken*)iden)->iden
					               ),
					               new ArrayList<AstNode*>() //变量无后缀，列表为空
					           ),
					           Ast<AstExpression>(	//右值（即null）
					               iden->lineNo,
					               Ast<AstBinary>(
					                   iden->lineNo,
					                   Ast<AstUnary>(
					                       iden->lineNo,
					                       Ast<AstNull>(
					                           iden->lineNo
					                       ),
					                       new ArrayList<AstNode*>() //null无后缀
					                   )
					               )
					           )

					       );
				}

				AstExpression* def_var() {
					//返回的是赋值语句

					Token* iden = matcher.Peek(0);

					if(iden->type!=TokenIden) {
						//变量名必须为标识符
						THROW("the name of the variable must be an identifier")
						return NULL;
					}

					scopes[scopes.size()-1].mark(iden);	//登记该变量

					if(matcher.Peek(1)->type==TokenAssign) {
						//初始化赋值
						AstExpression* expr = expression();
						CE
						return expr;
					}

					//没有初始化赋值，则赋空值

					_pop;		//弹出标识符

					return MakeNullExpression(iden);

				}

				AstDefFunc* def_func() {

					int lineNo = match(TokenFunc)->lineNo;	//行号

					AstIdentifierName* iden = IDEN();
					ArrayList<AstNode*>* args = new ArrayList<AstNode*>();

					if(check(TokenLBR)) {
						//有参数列表
						_pop;	//弹出左括号

						if(check(TokenIden)) {
							IdenToken* iden = (IdenToken*)_pop;
							args->add(
							    Ast<AstIdentifierName>(
							        iden->lineNo,
							        iden->iden
							    )
							);
						}

						while(check(TokenCmm)) {
							//只要后面还有参数
							_pop;	//弹出逗号
							IdenToken* iden = (IdenToken*)_pop;
							args->add(
							    Ast<AstIdentifierName>(
							        iden->lineNo,
							        iden->iden
							    )
							);
						}
					}

				}
				AstDefClass* def_class() {}
				AstIfStatement* statement_if() {}
				AstWhileStatement* statement_while() {}
				AstForStatement* statement_for() {}

				AstExpression* expression() {
					AstExpression* rst;
					AstBinary* val = binary_operator();
					CE
					if(check(TokenAssign)) {
						//看到等号
						AstLeftValue* left = left_value(val);	//解析成左值
						CE
						_pop;	//弹出等号
						AstExpression* right = expression();
						CE
						return Ast<AstExpression>(left->lineNo, left, right);
					} else {
						//单纯的右值表达式
						return Ast<AstExpression>(val->lineNo, val);
					}
				}

				AstLeftValue* left_value(AstBinary* val) {
					//用于将右值表达式转换为左值表达式（即AstBinary* -> AstLeftValue*）
					//如果无法转换，则报错

					/*
					 * 一个左值表达式的ast格式应该是
					 * binary_operator: -1
					 	* unary_operator: -1
						 	* quark: IDEN
							* postfix: 下标后缀或成员后缀
							* postfix: 下标后缀或成员后缀
							...
					*/

					AstNode* quark;
					ArrayList<AstNode*>* postfixs = new ArrayList<AstNode*>();

					if(val->getOperatorType()!=-1) {	//binary_operator: -1
						THROW("lvalue required as "
						      "left operand of assignment")
					}

					AstUnary* unary = (AstUnary*)val->Children()->at(0);

					if(unary->getOperatorType()!=-1) {	//unary_operator: -1
						THROW("lvalue required as "
						      "left operand of assignment")
					}

					ArrayList<AstNode*>* children = unary->Children();

					quark = children->at(0);

					//开始分析左值表达式

					if(quark->getType()!=AstIdentifierType) {
						//quark: IDEN
						THROW("lvalue required as "
						      "left operand of assignment")
					}

					CE

					for(int i=1,len=children->size(); i<len; i++) {
						//postfix: 下标后缀或成员后缀

						AstPostfix* p;		//转换成右值后缀
						p = (AstPostfix*)children->at(i);

						if(
						    p->getPostfixType()!=PostfixMemberType
						    &&p->getPostfixType()!=PostfixElementType
						) {	//如果不满足左值后缀条件
							THROW("lvalue required as "
							      "left operand of assignment")
						}


						AstLeftPostfix* tmp = Ast<AstLeftPostfix>(
						                          p->lineNo,
						                          p->getPostfixType(),
						                          p->Children()->at(0)
						                      );

						postfixs->add(tmp);


					}

					CE

					return Ast<AstLeftValue>(
					           quark->lineNo,
					           (AstIdentifierName*)quark,
					           postfixs
					       );

				}

				AstBinary* binary_operator() {
					AstNode* value = _bin_operator(1);
					CE
					return Ast<AstBinary>(value->lineNo, value);
				}

				AstNode* _bin_operator(int layer) {
					//双目运算符的主要分析算法在这里实现
					//这个函数是一个递归函数

					if(layer==11) {
						//递归边界
						return unary_operator();
					}

					AstNode* rst = _bin_operator(layer+1);
					CE

					Token* op = matcher.Peek(0);

					while (bin_layer[layer-1][0]<=op->type
					        &&op->type<=bin_layer[layer-1][1]) {
						//只有还有运算符
						_pop;
						AstNode* right = _bin_operator(layer+1);
						CE
						rst = Ast<AstBinary>(
						          op->lineNo,
						          op->type-37,
						          rst,
						          right
						      );
						//其中，op->type-37其实是将token里的运算符映射到ast中的运算符
						//例如TokenBitOR-37 = 2 = BinaryBitORType

						op = matcher.Peek(0);
					}

					return rst;
				}

				//双目运算符的迭代分析将会在后续实现

				AstUnary* unary_operator() {
					//判断是否还有前缀的单目运算符
					unary_check(TokenAdd, UnaryPositiveType)
					unary_check(TokenSub, UnaryNegative)
					unary_check(TokenBitNot, UnaryInverseType)
					unary_check(TokenLogNot, UnaryNotType)
					//如果没有，则直接返回quark { postfix }
					AstNode* q = quark();
					CE
					ArrayList<AstNode*>* postfixs = new ArrayList<AstNode*>();
					while(
					    check(TokenLBR)
					    ||check(TokenLSB)
					    ||check(TokenMember)
					) {
						//只要还有后缀运算符，就匹配
						postfixs->add(postfix());
						CE
					}
					return Ast<AstUnary>(q->lineNo, q, postfixs);
				}

				AstPostfix* postfix() {
					if(check(TokenLBR)) {
						//调用函数后缀
						AstArguments* rst = arguments();
						CE
						return Ast<AstPostfix>(
						           rst->lineNo , PostfixCallType, rst
						       );
					}
					if(check(TokenLSB)) {
						//下标后缀
						int line = _pop->lineNo;
						AstExpression* expr = expression();
						CE
						match(TokenRSB);
						return Ast<AstPostfix>(
						           line, PostfixElementType, (AstNode*)expr
						       );
					}
					if(check(TokenMember)) {

						int line = _pop->lineNo;

						//还要特判是不是.new
						if(check(TokenNew)) {
							match(TokenNew);
							//特性支持：如果新建对象的构造函数无参数，可以省略括号
							if(check(TokenLBR)==false) {
								return Ast<AstPostfix> (
								           line,
								           PostfixNewType,
								           (AstNode*) new AstArguments(
								               new ArrayList<AstNode*>()
								           )
								           //空的参数列表
								       );
							} else {
								//正常的.new(...)
								AstArguments* rst = arguments();
								CE
								return Ast<AstPostfix> (
								           rst->lineNo ,
								           PostfixNewType, (AstNode*)rst
								       );
							}
						}

						//正常的访问成员
						IdenToken* iden_tok = (IdenToken*)match(TokenIden);
						CTH("the member name must be an identifier")
						AstIdentifierName* iden = Ast<AstIdentifierName>(
						                              iden_tok->lineNo,
						                              iden_tok->iden
						                          );
						return Ast<AstPostfix>(line, PostfixMemberType, iden);
					}
					THROW("invalid syntax")
					return NULL;
				}

				AstArguments* arguments() {
					ArrayList<AstNode*>* exprs = new ArrayList<AstNode*>();
					int line = match(TokenLBR)->lineNo;
					CE
					if(check(TokenRBR)==false) {
						//左括号后没有紧跟着右括号，代表有传参数
						exprs->add(expression());
						CE
					}
					while(check(TokenRBR)==false) {
						//只要没有匹配到右括号
						match(TokenCmm);
						CE
						exprs->add(expression());
						CE
					}
					_pop;	//弹出右括号
					return Ast<AstArguments>(line, exprs);
				}

				AstNode* quark() {
					if(check(TokenIden)) {
						return IDEN();
					}
					if(
					    check(TokenInt)||check(TokenDouble)
					    ||check(TokenTrue)||check(TokenFalse)
					) {	//分析一个数字

						return NUM();
					}
					if(check(TokenString)) {
						return STR();
					}
					if(check(TokenNull)) {
						return _NULL();
					}
					if(check(TokenLBC)) {
						return list_literal();
					}
					if(check(TokenLSB)) {
						return array_literal();
					}
					if(check(TokenLBR)) {
						_pop;
						AstExpression* expr = expression();
						CE
						match(TokenRBR);
						CTH("the parentheses are not closed")
						return expr;
					}
					CE
					THROW("invalid syntax")
					return NULL;
				}

				//注意：这里的IDEN()只有在匹配变量名的时候能使用
				//如果是匹配类成员名等，请自行建立AstIdentifierName节点
				AstIdentifierName* IDEN() {
					//分析一个标识符
					IdenToken* tok = (IdenToken*)match(TokenIden);

					if(scopes[scopes.size()-1].exist(tok)==false) {
						//未声明的标识符
						THROW("undefined variable")
						return NULL;
					}

					return Ast<AstIdentifierName>(tok->lineNo, tok->iden);
				}

				AstNumber* NUM() {
					if(check(TokenInt)) {
						IntToken* tok = (IntToken*)_pop;
						return Ast<AstIntNumber>(tok->lineNo, tok->val);
					} else if(check(TokenDouble)) {
						DoubleToken* tok = (DoubleToken*)_pop;
						return Ast<AstDoubleNumber>(tok->lineNo, tok->val);
					} else if(check(TokenTrue)) { //布尔值在语法分析时会被转换成1和0
						return Ast<AstIntNumber>(_pop->lineNo, 1);
					} else if(check(TokenFalse)) {
						return Ast<AstIntNumber>(_pop->lineNo, 0);
					} else {
						THROW("invalid syntax")
					}
					return NULL;
				}

				AstString* STR() {
					StringToken* tok = (StringToken*)match(TokenString);
					return Ast<AstString>(tok->lineNo, tok->content);
				}

				AstNull* _NULL() {
					Token* t = match(TokenNull);
					CE
					return Ast<AstNull>(
					           t->lineNo
					       );
				}

				AstArrayLiteral* array_literal() {
					Token* t = match(TokenLSB);
					AstArrayLiteral* rst = Ast<AstArrayLiteral>(
					                           t->lineNo,
					                           expression()
					                       );
					match(TokenRSB);
					CTH("the square parentheses are not closed")
					return rst;
				}

				AstListLiteral* list_literal() {
					Token* t = match(TokenLBC);
					ArrayList<AstNode*>* exprs = new ArrayList<AstNode*>();

					if(check(TokenRBC)) {
						//如果列表为空
						_pop;
						return Ast<AstListLiteral>(t->lineNo, exprs);
					}

					exprs->add((AstNode*)expression());
					CE
					while(check(TokenCmm)) {
						_pop;
						exprs->add((AstNode*)expression());
						CE
					}

					AstListLiteral* rst = Ast<AstListLiteral>(t->lineNo, exprs);

					match(TokenRBC);
					CTH("the parentheses are not closed")
					return rst;
				}
		};
	}
}

/*

实例：

class c {
	func helloworld(this) {
		print("Hello world!");
	}
}

def main = func {
	def cls = c.new;
	cls.helloworld();
}();

类 输出类 {
	函数 你好世界（后台） {
		输出（“Hello world!”）；
	}
}

设 主 = 函数() {
	设 类对象 = 输出类——新建；
	类对象——你好世界();
}

类 输出类 {
	函数 你好世界（后台） {
		输出（“Hello world!”）；
	}
}

设 主 = 函数() {
	设 类对象 = 输出类——新建；
	类对象——你好世界（）；
}
*/

#undef check
#undef pop
#undef CE

#undef unary_check

#endif