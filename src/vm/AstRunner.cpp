/*
	Name: ast::AstRunner.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 11/02/24 14:16
	Description: 语法树的运行器
*/

#pragma once

#include"ArrayList.hpp"
#include"AstRunnerException.cpp"
#include"String.hpp"
#include"StamonLib.hpp"
#include"BasicPlatform.hpp"
#include"Ast.hpp"
#include"DataType.hpp"
#include"TypeCalculator.cpp"
#include"ObjectManager.cpp"
#include"AstIr.cpp"
#include"SFN.cpp"
#include"Parser.cpp"

#define CDT(dt, type) \
	if(dt->getType()!=type##ID) {\
		THROW(\
			exception::astrunner::TypeError(\
				POSITION, getDataTypeName(dt->getType())\
			)\
		);\
	}\
	CE

//强制检查数据类型（Check Data Type）

#define OPND_PUSH(data)	manager->OPND.add(data);
#define OPND_POP manager->OPND.erase(manager->OPND.size()-1);
#define RUN(ast) runAst(ast); CE
#define GETDT(var, code) \
	if(true) {\
		RetStatus tmp = (code);\
		CE\
		var = tmp.retval->data;\
	}
//Get datatype::DataType，想要安全的获取datatype::DataType，应该使用这个宏

#define CE			CATCH { return RetStatus(RetStatusErr, NullVariablePtr()); }
//如果执行代码中出现异常，则抛出新的异常并覆盖原本的异常
#define CTH(message)	CATCH { THROW(message) }
//用于绑定函数指针
#define BIND(name) RunAstFunc[ast::Ast##name##Type] = &AstRunner::run##name;

#define POSITION getExecutePosition()

namespace stamon::vm {

	enum RET_STATUS_CODE {	//返回的状态码集合
	    RetStatusErr = -1,	//错误退出（Error）
	    RetStatusNor,		//正常退出（Normal）
	    RetStatusCon,		//继续循环（Continue）
	    RetStatusBrk,		//退出循环（Break）
	    RetStatusRet		//函数返回（Return）
	};

	/*
	 * 由于在运行过程中，传递RetStatus就会传递Variable*
	 * 这些Variable*中：
	 	* 有些是为了传递而生成的（即临时Variable*）
		* 有些是归ObjectManager管理的（即左值Variable*）
		* 有些是不用传递的空指针（即空Variable*）
	 * 为了保障将临时Variable*在合适的时间删除的同时
	 * 不让空Variable*和左值Variable*被删除
	 * 我使用了智能指针
	 * 具体方式就是定义三个继承自智能指针类的子类：
	 	* 左值Variable*（即LeftVariablePtr）
		* 临时Variable*（即RightVariablePtr）
		* 空Variable*（即NullVariablePtr）
	 * 这三个子类会在初始化时指定销毁方式，只有RightVariablePtr会真正销毁指针
	 * 利用这种方式，就能让AstRunner在运行过程中及时清理掉垃圾内存，减小内存泄漏
	 */

	using VariablePtr = SmartPtr<Variable>;
	//为了简写，使用using关键字定义（效果类似typedef）

	class LeftVariablePtr : public VariablePtr {
		public:
		LeftVariablePtr(Variable* ptr)
		: VariablePtr(ptr, stamon::DestroyNothing) {}
		//左值变量指针不需要被销毁
	};

	class RightVariablePtr : public VariablePtr {
		public:
		RightVariablePtr(datatype::DataType* ptr)
		: VariablePtr(new Variable(ptr)) {}
		//右值变量指针需要被销毁
	};

	class NullVariablePtr : public VariablePtr {
		public:
		NullVariablePtr()
		: VariablePtr(NULL, stamon::DestroyNothing) {}
		//空变量指针不需要被销毁
	};

	class RetStatus {	//返回的状态（Return Status）
			//这个类用于运行时
		public:
			int status;	//状态码
			VariablePtr retval;	//返回值（Return-Value），无返回值时为NULL
			RetStatus() : retval(NullVariablePtr()) {}
			RetStatus(const RetStatus& right) : retval(right.retval) {
				status = right.status;
			}
			RetStatus(int status_code, VariablePtr retvalue) 
			: retval(retvalue) {
				status = status_code;
			}
	};

	class AstRunner {

		public:
			ObjectManager* manager;
			RetStatus(AstRunner::*RunAstFunc[ast::AstTypeNum])
			(ast::AstNode* node);
			//由类成员函数指针组成的数组
			ast::AstNode* program;
			bool is_gc;	//是否允许gc
			int gc_mem_limit;	//对象内存最大限制
			ArrayList<datatype::DataType*> tabconst;	//常量表
			ArrayList<String> vm_args;	//虚拟机参数
			log::Exception* ex;	//异常
			sfn::SFN sfn;
			TypeCalculator typecalculator;

			int RunningLineNo;
			String RunningFileName;

			AstRunner() {
				BIND(Program)
				BIND(DefClass)
				BIND(DefFunc)
				BIND(DefVar)
				BIND(AnonClass)
				BIND(AnonFunc)
				BIND(Block)
				BIND(Break)
				BIND(Continue)
				BIND(IfStatement)
				BIND(WhileStatement)
				BIND(ForStatement)
				BIND(ReturnStatement)
				BIND(SFN)
				BIND(Expression)
				BIND(LeftValue)
				BIND(Binary)
				BIND(Unary)
				BIND(Leaf)
				BIND(Null)
				BIND(ArrayLiteral)
				BIND(ListLiteral)
			}

			String getDataTypeName(int type);
			String getExecutePosition();

			RetStatus execute(
			    ast::AstNode* main_node, bool isGC, int vm_mem_limit,
			    ArrayList<datatype::DataType*> tableConst,
			    ArrayList<String> args, int pool_cache_size, log::Exception* e
			) {

				//初始化参数
				program = main_node;
				is_gc = isGC;
				gc_mem_limit = vm_mem_limit;
				tabconst = tableConst;
				vm_args = args;

				ex = e;
				//初始化对象管理器
				manager = new ObjectManager(is_gc, vm_mem_limit,
											pool_cache_size, ex);

				sfn = sfn::SFN(e, manager);

				typecalculator = TypeCalculator(ex, manager);

				//执行程序
				auto st = runAst(program);
				//释放内存

				delete manager;

				return st;
			}

			RetStatus runAst(ast::AstNode* node) {
				RunningLineNo = node->lineNo;
				RunningFileName = node->filename;

				if(node->getType()==-1) {
					//叶子节点
					return runLeaf(node);
				} else {
					return (this->*(RunAstFunc[node->getType()]))(node);				
				}
			}

			RetStatus runProgram(ast::AstNode* node) {
				manager->pushScope();	//全局作用域

				for(int i=0,len=node->Children()->size(); i<len; i++) {
					auto st = RUN(node->Children()->at(i));
					if(st.status!=RetStatusNor) {
						if(st.status==RetStatusRet) {
							THROW(exception::astrunner::ReturnError(POSITION));
						} else if(st.status==RetStatusBrk) {
							THROW(exception::astrunner::BreakError(POSITION));
						} else if(st.status==RetStatusCon) {
							THROW(
								exception::astrunner::ContinueError(POSITION)
							);
						}
						CE
					}

				}

				manager->popScope();

				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			datatype::ObjectType* initObject(datatype::ClassType* cls) {
				datatype::ObjectType* rst;
				HashMap<int, Variable*> membertab;	//成员表
				auto node = cls->getVal();

				/*处理父类*/
				if(node->father_flag == true) {
					//有父类，先初始化父类
					auto father_class = manager->getVariable(
					                        (
					                            (ast::AstIdentifier*)
					                            node
					                            ->Children()
					                            ->at(0)
					                        )
					                        ->getID()
					                    );
					CATCH {
						return NULL;
					}
					if(father_class->data->getType()!=datatype::ClassTypeID) {
						THROW(
							exception
							::astrunner
							::TypeError(
								POSITION,
								getDataTypeName(father_class->data->getType())
							)
						);
						return NULL;
					}

					rst = initObject(
						(datatype::ClassType*)(father_class->data)
					);
					CATCH {
						return NULL;
					}

					membertab = rst->getVal();

				} else {
					rst = manager
					      ->MallocObject<datatype::ObjectType>(membertab);

					CATCH {
						return NULL;
					}
				}

				OPND_PUSH(rst);

				/*接着继续初始化*/
				manager->pushMemberTabScope(ObjectScope(membertab));
				//将membertab注入到新的作用域当中
				//这样所有的操作都会直接通过membertab反馈到rst

				for(int i=0,len=node->Children()->size(); i<len; i++) {
					if(i==0 && node->father_flag) {
						//如果有父类，则要从第二个节点开始
						continue;
					}

					auto n = node->Children()->at(i); //用n代替这个复杂的表达式

					if(n->getType()==ast::AstDefClassType) {
						runDefClass(n);
					} else if(n->getType()==ast::AstDefFuncType) {
						runDefMethod(rst, n);
					} else if(n->getType()==ast::AstDefVarType) {
						runDefVar(n);
					}

					CATCH {
						return NULL;
					}

				}

				/*收尾*/
				OPND_POP	//弹出rst
				manager->popMemberTabScope();

				return rst;
			}

			RetStatus runMethod(
			    datatype::MethodType* method, ArrayList<ast::AstNode*>* args
			) {
				OPND_PUSH(method);

				/*先获取容器*/
				auto container = method->getContainer();

				/*再获得形参*/
				auto FormArg = *(method->getVal()->Children());
				FormArg.erase(FormArg.size()-1);	//删除ast::AstBlock，只保留参数

				/*接着计算实际参数的表达式*/
				ArrayList<datatype::DataType*> args_val;

				if(container!=NULL) {
					//说明该函数有容器，即该函数是方法
					//规定方法的第一个参数为容器
					args_val.add(container);
					OPND_PUSH(container)
				}

				for(int i=0,len=args->size(); i<len; i++) {

					auto st = RUN(args->at(i));	//计算参数

					args_val.add(st.retval->data);	//加入参数表

					OPND_PUSH(args_val[args_val.size()-1]);	//压入参数
				}

				for(int i=0,len=args_val.size(); i<len; i++) {
					OPND_POP	//弹出参数
				}

				/*然后对比形参和实参的个数*/
				if(FormArg.size()!=args_val.size()) {
					//形式参数个数要等于实际参数个数
					THROW(
						exception
						::astrunner
						::ArgumentsError(
							POSITION,
							toString(FormArg.size()),
							toString(args_val.size())
						)
					);
					CE
				}

				/*新建作用域*/
				manager->pushScope();

				/*如果有函数名，就存入它*/
				if(method->id!=-1) {
					manager->NewVariable(method->id, method);
				}

				/*存入参数*/
				for(int i=0,len=FormArg.size(); i<len; i++) {
					manager->NewVariable(
					    (
					        (ast::AstIdentifier*)FormArg.at(i)
					    )
					    ->getID(),
					    args_val[i]
					);
				}

				/*执行函数体*/
				auto st = RUN(
				              method
				              ->getVal()
				              ->Children()
				              ->at(FormArg.size())
				          );
				/*
				 * 这里用到了一些简便思路
				 * 我想要运行函数体（即ast::AstBlock）
				 * 函数体位于Children()的最后一个元素
				 * 如果直接暴力的访问最后一个元素，那么代码要写为
				 * method->getVal()->Children()->at(
				 * 		method->getVal()->Children->size()-1
				 * )
				 * 这样太复杂了
				 * 我注意到：
				 * 		FormArg.size()
				 * 		= method->getVal()->Children->size()-1
				 * 所以以FormArg.size()访问函数体
				 */

				/*弹出作用域*/
				manager->popScope();
				OPND_POP	//弹出method

				/*返回*/
				if(st.status==RetStatusRet) {
					//有返回值
					return RetStatus(RetStatusNor, st.retval);
				}
				//无返回值，返回rst（即null）
				return RetStatus(RetStatusNor, RightVariablePtr(
												manager->MallocObject
														 <datatype::NullType>()
											   )
								);
			}

			RetStatus runBlock(ast::AstNode* node) {
				auto blk_node = (ast::AstBlock*)node;
				ArrayList<ast::AstNode*>* codes = node->Children();

				for(int i=0,len=codes->size(); i<len; i++) {
					auto st = RUN(codes->at(i));
					if(
					    st.status==RetStatusBrk
					    || st.status==RetStatusCon
					    || st.status==RetStatusRet
					) {
						return st;
					}
				}

				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			RetStatus runDefVar(ast::AstNode* node) {
				auto val = RUN(node->Children()->at(1));
				manager->NewVariable(
				    (
				        (ast::AstIdentifier*)node->Children()->at(0)
				    )
				    ->getID(),
				    val.retval->data
				);
				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			datatype::MethodType* runDefMethod(
			    datatype::ObjectType* container, ast::AstNode* node) {

				auto iden = (
				                (ast::AstIdentifier*)node->Children()->at(0)
				            )
				            ->getID();

				RetStatus st = runAst(node->Children()->at(1));

				CATCH {
					return NULL;
				}

				auto func = (datatype::MethodType*)st.retval->data;

				func->id = iden;
				func->container = container;

				manager->NewVariable(iden, func);

				return func;
			}

			RetStatus runDefFunc(ast::AstNode* node) {
				auto iden = (
				                (ast::AstIdentifier*)node->Children()->at(0)
				            )
				            ->getID();

				auto st = RUN(node->Children()->at(1));

				auto func = (datatype::MethodType*)st.retval->data;

				func->id = iden;

				manager->NewVariable(iden, func);

				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			RetStatus runDefClass(ast::AstNode* node) {
				auto iden = (
				                (ast::AstIdentifier*)node->Children()->at(0)
				            )
				            ->getID();
				auto st = RUN(node->Children()->at(1));

				auto cls = (datatype::ClassType*)st.retval->data;

				manager->NewVariable(iden, cls);

				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			RetStatus runAnonClass(ast::AstNode* node) {
				auto ancl_node = (ast::AstAnonClass*)node;
				auto rst = manager
						   ->MallocObject<datatype::ClassType>
						   (ancl_node);
				CE;

				return RetStatus(
				           RetStatusNor,
				           RightVariablePtr(rst)
				       );
			}

			RetStatus runAnonFunc(ast::AstNode* node) {
				auto anfc_node = (ast::AstAnonFunc*)node;
				auto rst = manager
						   ->MallocObject<datatype::MethodType>
						   (
						   		-1, anfc_node, (datatype::ObjectType*)NULL
						   );
				CE;

				return RetStatus(
				           RetStatusNor,
				           RightVariablePtr(rst)
				       );
			}

			RetStatus runForStatement(ast::AstNode* node) {
				auto stm_node = (ast::AstForStatement*)node;

				int iden = (
				               (ast::AstIdentifier*)
				               node->Children()->at(0)
				           )
				           ->getID();	//遍历的标识符

				RetStatus list_st = RUN(node->Children()->at(1));

				datatype::DataType* list_dt = list_st.retval->data;

				OPND_PUSH(list_dt)

				CDT(list_dt, datatype::SequenceType)

				auto list = ((datatype::SequenceType*)list_dt)->getVal();

				for(int i=0; i<list.size(); i++) {

					manager->pushScope();

					//把迭代变量放到作用域当中
					manager->NewVariable(
						iden, list[i]->data
					);

					//每次都新建一个标识符

					auto st = RUN(node->Children()->at(2));

					if(st.status==RetStatusBrk) {
						return RetStatus(RetStatusNor, NullVariablePtr());
					}

					if(st.status==RetStatusRet) {
						return st;
					}

					manager->popScope();

				}

				OPND_POP	//弹出list_dt

				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			RetStatus runWhileStatement(ast::AstNode* node) {
				auto stm_node = (ast::AstWhileStatement*)node;

				RetStatus cond_st = RUN(node->Children()->at(0));

				datatype::DataType* cond = cond_st.retval->data;

				OPND_PUSH(cond)

				manager->pushScope();

				while(typecalculator.toBool(cond)==true) {
					RetStatus st = RUN(node->Children()->at(1));

					if(st.status==RetStatusBrk) {
						OPND_POP;	//弹出cond
						return RetStatus(RetStatusNor, NullVariablePtr());
					}
					if(st.status==RetStatusRet) {
						OPND_POP;	//弹出cond
						return st;
					}

					OPND_POP	//弹出cond
					manager->popScope();

					RetStatus cond_st = RUN(node->Children()->at(0));
					cond = cond_st.retval->data;

					OPND_PUSH(cond)
					manager->pushScope();
				}

				OPND_POP	//弹出cond
				manager->popScope();

				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			RetStatus runIfStatement(ast::AstNode* node) {
				auto stm_node = (ast::AstIfStatement*)node;

				RetStatus cond_st = RUN(node->Children()->at(0));

				datatype::DataType* cond = cond_st.retval->data;

				OPND_PUSH(cond)

				RetStatus st;

				if(typecalculator.toBool(cond)==true) {

					manager->pushScope();

					st = RUN(node->Children()->at(1));

					manager->popScope();

				} else if(node->Children()->size()==3) {

					//有三个子节点，证明有else代码块

					manager->pushScope();

					st =RUN(node->Children()->at(2));

					manager->popScope();

				} else {
					//直接略过本代码块
					st = RetStatus(RetStatusNor, NullVariablePtr());
				}

				if(st.status==RetStatusBrk) {
					THROW(exception::astrunner::BreakError(POSITION));
					CE
				}

				if(st.status==RetStatusCon) {
					THROW(exception::astrunner::ContinueError(POSITION));
					CE
				}

				if(st.status==RetStatusRet) {
					return st;
				}

				OPND_POP	//弹出cond

				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			RetStatus runReturnStatement(ast::AstNode* node) {
				auto st = RUN(node->Children()->at(0));
				return RetStatus(RetStatusRet, st.retval);
			}

			RetStatus runSFN(ast::AstNode* node) {
				int port = ((ast::AstIdentifier*)node->Children()->at(0))
				           ->getID();
				int arg = ((ast::AstIdentifier*)node->Children()->at(1))
				          ->getID();

				Variable* port_var = manager->getVariable(port);
				CE
				Variable* arg_var = manager->getVariable(arg);
				CE

				CDT(port_var->data, datatype::StringType)

				OPND_PUSH(port_var->data);
				OPND_PUSH(arg_var->data);

				sfn.call(
				    (
				        (datatype::StringType*)port_var->data
				    )
				    ->getVal(),
				    arg_var
				);
				CE

				OPND_POP;
				OPND_POP;

				return RetStatus(RetStatusNor, NullVariablePtr());
			}


			RetStatus runExpression(ast::AstNode* node) {
				auto expr_node = (ast::AstExpression*)node;
				if(expr_node->ass_type==-1) {
					return runAst(node->Children()->at(0));
				}

				//如果是赋值表达式

				//先得出左右两式
				VariablePtr left_value = runAst(node->Children()->at(0))
				                       .retval;
				CE

				VariablePtr right_value = runAst(node->Children()->at(1))
				                        .retval;
				CE

				if(expr_node->ass_type==c::TokenAssign) {
					//直接赋值
					left_value->data = right_value->data;
					return RetStatus(RetStatusNor, left_value);
				}

				OPND_PUSH(left_value->data);
				OPND_PUSH(right_value->data);

				//特判错误情况

				if(
					(
						expr_node->ass_type==c::TokenDivAss
						|| expr_node->ass_type==c::TokenModAss
					)
					&& typecalculator.toBool(right_value->data)==false
				) {
					//除数为0
					THROW(exception::astrunner::ZeroDivisionError(POSITION));
					CE;
				}

				int optype = typecalculator.AssignOperatorToBinaryOperator(
												expr_node->ass_type
											);
				
				if(optype==-1) {
					//符号有误
					THROW(exception::astrunner::UnknownOperatorError(POSITION));
					CE;
				}

				if(typecalculator.checkBinaryOperandType(
									left_value->data, optype, right_value->data
								  )
					==false
				) {
					//运算类型有误
					THROW(
						exception
						::astrunner
						::TypeError(
							POSITION,
							toString(left_value->data->getType())
						)
					);
					CE;
				}

				//开始运算

				left_value->data = typecalculator.BinaryOperate(
											left_value->data,
											optype,
											right_value->data
								   );
				
				OPND_POP;
				OPND_POP;

				return RetStatus(RetStatusNor, left_value);

			}

			RetStatus runLeftValue(ast::AstNode* node) {
				auto lv_node = (ast::AstLeftValue*)node;
				//获取标识符
				VariablePtr lvalue = LeftVariablePtr(
											manager->getVariable(
											(
											(ast::AstIdentifier*)
											node->Children()->at(0)
											)
											->getID()
				                   		)
									);
				CE;
				for(int i=1,len=node->Children()->size(); i<len; i++) {
					//分析后缀
					lvalue = runPostfix(
					             node->Children()->at(i), lvalue->data
					         )
					         .retval;
					CE
				}
				return RetStatus(RetStatusNor, lvalue);
			}

			RetStatus runBreak(ast::AstNode* node) {
				return RetStatus(RetStatusBrk, NullVariablePtr());
			}

			RetStatus runContinue(ast::AstNode* node) {
				return RetStatus(RetStatusCon, NullVariablePtr());
			}

			RetStatus runBinary(ast::AstNode* node) {
				ast::AstBinary* bin_node = (ast::AstBinary*)node;
				int optype = bin_node->operator_type;

				if(optype==-1) {
					RetStatus st = RUN(bin_node->Children()->at(0));
					return st;
				}

				//获取运算数

				RetStatus left_st = RUN(bin_node->Children()->at(0));
				datatype::DataType* left = left_st.retval->data;
				OPND_PUSH(left);

				RetStatus right_st = RUN(bin_node->Children()->at(1));
				datatype::DataType* right = right_st.retval->data;
				OPND_PUSH(right);

				//先特判运算符和运算类型是否合法
				if(typecalculator.checkBinaryOperator(optype)==false) {
					THROW(exception::astrunner::UnknownOperatorError(POSITION));
					CE;
				}
				
				if(
					typecalculator.checkBinaryOperandType(
									left, optype, right
								  )
					== false
				) {
					THROW(
						exception
						::astrunner
						::TypeError(
							POSITION,
							toString(left->getType())
						)
					);
					CE;
				}

				//再判断位移、除法的数值是否合法

				if(
					(
						optype==ast::BinaryLeftShiftType
						||optype==ast::BinaryRightShiftType
					)
					&& typecalculator.toInt(right) < 0
				) {
					//位移数不能为负
					THROW(exception::astrunner::NegativeShiftError(POSITION));
					CE;
				}

				if(
					(
						optype==ast::BinaryDiviType
						||optype==ast::BinaryModType
					)
					&& typecalculator.toBool(right)==false
				) {
					//除数不能为0
					//将b转为布尔值，如果b是0，则一定为false
					THROW(exception::astrunner::ZeroDivisionError(POSITION));
					CE;
				}
				
				datatype::DataType* rst = typecalculator.BinaryOperate(
															left, optype, right
														 );

				OPND_POP;
				OPND_POP;

				return RetStatus(RetStatusNor, RightVariablePtr(rst));

			}

			RetStatus runUnary(ast::AstNode* node) {
				ast::AstUnary* unary_node = (ast::AstUnary*)node;

				if(unary_node->operator_type==-1) {
					//先分析quark
					datatype::DataType* quark;
					GETDT(quark, runAst(node->Children()->at(0)))
					OPND_PUSH(quark);

					//接着逐个分析后缀
					for(int i=1,len=node->Children()->size(); i<len; i++) {
						GETDT(
						    quark,
						    runPostfix(node->Children()->at(i), quark)
						)
						OPND_POP;
						OPND_PUSH(quark);
					}

					OPND_POP;

					return RetStatus(RetStatusNor, RightVariablePtr(quark));
				}

				//如果是单目运算符

				datatype::DataType* src;
				int optype = unary_node->operator_type;

				GETDT(src, runAst(node->Children()->at(0)));

				OPND_PUSH(src);

				//特判运算符和运算类型是否合法
				if(typecalculator.checkUnaryOperator(optype)==false) {
					THROW(exception::astrunner::UnknownOperatorError(POSITION));
					CE;
				}
				
				if(
					typecalculator.checkUnaryOperandType(src, optype) == false
				) {
					THROW(
						exception
						::astrunner
						::TypeError(
							POSITION,
							toString(src->getType())
						)
					);
					CE;
				}

				datatype::DataType* rst = typecalculator
										  .UnaryOperate(src, optype);

				OPND_POP;

				return RetStatus(RetStatusNor, RightVariablePtr(rst));

			}

			RetStatus runPostfix(ast::AstNode* node, datatype::DataType* src) {
				ast::AstPostfix* postfix_node = (ast::AstPostfix*)node;
				int postfix_type = postfix_node->postfix_type;

				if(postfix_type==ast::PostfixMemberType) {
					//访问成员
					CDT(src, datatype::ObjectType)
					ast::AstIdentifier* member = (ast::AstIdentifier*)
					                             postfix_node
					                             ->Children()
					                             ->at(0);

					int member_id = member->getID();

					if(
					    ((datatype::ObjectType*)src)
					    ->getVal()
					    .exist(member_id)==false
					) {
						//未知成员
						THROW(exception::astrunner::UnknownMemberError(
							POSITION, toString(member_id))
						);
						CE
					}

					return RetStatus(
					           RetStatusNor, LeftVariablePtr(
									((datatype::ObjectType*)src)	//类对象
									->getVal()	//获取成员表
									.get(member_id)	//获取成员
							   )
					       );
				}

				if(postfix_type==ast::PostfixElementType) {
					//取下标
					CDT(src, datatype::SequenceType)
					ast::AstExpression* expr = (ast::AstExpression*)
					                           postfix_node
					                           ->Children()
					                           ->at(0);
					RetStatus st = RUN(expr);

					VariablePtr index_var = st.retval;

					datatype::DataType* index_dt = index_var->data;

					CDT(index_dt, datatype::IntegerType)

					const ArrayList<Variable*>& list 
									= ((datatype::SequenceType*)src)->getVal();

					int index = ((datatype::IntegerType*)index_dt)->getVal();

					if(index<0 ||index>=list.size()) {
						THROW(exception::astrunner::IndexError(POSITION));
						return RetStatus(RetStatusErr, NullVariablePtr());
					}

					return RetStatus(RetStatusNor, LeftVariablePtr(
														list[index]
												   )
									);

				}

				if(postfix_type==ast::PostfixNewType) {
					CDT(src, datatype::ClassType)

					ast::AstArguments* arg_ast = (ast::AstArguments*)
					                             postfix_node
					                             ->Children()
					                             ->at(0);
					ArrayList<ast::AstNode*>* arg = arg_ast->Children();

					OPND_PUSH(src)

					datatype::ObjectType* obj_dt = initObject(
					                                   (datatype::ClassType*)src
					                               );
					CE

					OPND_POP

					OPND_PUSH(obj_dt)

					if(obj_dt->getVal().exist(0)) {
						//有构造函数
						datatype::DataType* init_func_dt = obj_dt
						                                   ->getVal()
						                                   .get(0)
						                                   ->data;
						CDT(init_func_dt, datatype::MethodType)
						runMethod((datatype::MethodType*)init_func_dt, arg);
						CE
					}

					OPND_POP

					return RetStatus(RetStatusNor, RightVariablePtr(obj_dt));

				}

				if(postfix_type==ast::PostfixCallType) {
					//调用函数
					CDT(src, datatype::MethodType);
					ast::AstArguments* arg_ast = (ast::AstArguments*)
					                             postfix_node
					                             ->Children()
					                             ->at(0);
					ArrayList<ast::AstNode*>* arg = arg_ast->Children();

					OPND_PUSH(src)

					auto st = runMethod((datatype::MethodType*)src, arg);
					CE

					OPND_POP

					return st;
				}

				THROW(exception::astrunner::PostfixError(POSITION));
				return RetStatus(RetStatusErr, NullVariablePtr());
			}

			RetStatus runArrayLiteral(ast::AstNode* node) {
				ast::AstArrayLiteral* literal = (ast::AstArrayLiteral*)node;

				//获得长度
				RetStatus st = RUN(literal->Children()->at(0));

				datatype::DataType* length = st.retval->data;
				CDT(length, datatype::IntegerType)

				if(((datatype::IntegerType*)length)->getVal()<0) {
					//错误的数列长度
					THROW(exception::astrunner::LengthError(POSITION));
					CE;
				}

				OPND_PUSH(length)

				VariablePtr rst_var = RightVariablePtr(
				    manager->MallocObject<datatype::SequenceType>(
				        ((datatype::IntegerType*)length)->getVal()
				    )
				);

				CE

				for(
				    int i=0,len=((datatype::IntegerType*)length)->getVal();
				    i<len;
				    i++
				) {
					((datatype::SequenceType*)rst_var->data)->sequence[i]
					    = new Variable(
					    manager->MallocObject<datatype::NullType>()
					);
				}

				CE

				OPND_POP

				return RetStatus(RetStatusNor, rst_var);
			}

			RetStatus runListLiteral(ast::AstNode* node) {

				ast::AstListLiteral* literal = (ast::AstListLiteral*)node;
				ArrayList<Variable*> content;

				for (
				    int i=0,len=literal->Children()->size();
				    i<len;
				    i++
				) {
					RetStatus st = RUN(literal->Children()->at(i));

					datatype::DataType* item = st.retval->data;
					OPND_PUSH(item)
					content.add(new Variable(item));
				}


				VariablePtr rst_var = RightVariablePtr(
				    manager->MallocObject<datatype::SequenceType>(
				        content
				    )
				);

				CE

				for (
				    int i=0,len=literal->Children()->size();
				    i<len;
				    i++
				) {
					OPND_POP
				}	//弹出所有计算中的数据


				return RetStatus(RetStatusNor, rst_var);
			}

			RetStatus runIden(ast::AstNode* node) {
				int index = ((ast::AstLeaf*)node)->getVal();

				if(index>=tabconst.size()) {
					THROW(exception::astrunner::ConstantsError(POSITION));
					return RetStatus(RetStatusErr, NullVariablePtr());
				}

				datatype::DataType* rst = tabconst[index];

				CDT(rst, ir::IdenConstType)

				return RetStatus(RetStatusNor, RightVariablePtr(rst));
			}

			RetStatus runLeaf(ast::AstNode* node) {
				int index = ((ast::AstLeaf*)node)->getVal();
				if(index>=tabconst.size()) {
					THROW(exception::astrunner::ConstantsError(POSITION));
					return RetStatus(RetStatusErr, NullVariablePtr());
				}

				datatype::DataType* rst = tabconst[index];

				if(rst->getType()==-1) {
					//标识符

					return RetStatus(
					           RetStatusNor, LeftVariablePtr(
												manager->getVariable(index)
											 )
					       );
				}

				return RetStatus(
				           RetStatusNor, RightVariablePtr(rst)
				       );
			}

			RetStatus runNull(ast::AstNode* node) {
				return RetStatus(
				           RetStatusNor,
				           RightVariablePtr(
				               manager->MallocObject<datatype::NullType>()
				           )
				       );
			}

	};

	//一些冗余的函数放到后面

	inline String AstRunner::getDataTypeName(int type) {
		switch (type)
		{
		case stamon::datatype::ClassTypeID:		return String("class");
		case stamon::datatype::MethodTypeID:	return String("function");
		case stamon::datatype::IntegerTypeID:	return String("int");
		case stamon::datatype::FloatTypeID:		return String("float");
		case stamon::datatype::DoubleTypeID:	return String("double");
		case stamon::datatype::ObjectTypeID:	return String("object");
		case stamon::datatype::SequenceTypeID:	return String("sequence");
		case stamon::datatype::StringTypeID:	return String("string");
		case -1:								return String("identifier");
		default:								return String("unknown-type");
		}
	}

	inline String AstRunner::getExecutePosition() {
		if (RunningFileName.equals("") && RunningLineNo == -1) {
			// 字节码中被没有调试信息，即被字节码被strip过
			return String("");
		} else {
			return String("at \"") + RunningFileName + String("\": ")
				+ toString(RunningLineNo) + String(": ");
		}
	}
} //namespace stamon::vm

#undef CDT
#undef OPND_PUSH
#undef OPND_POP
#undef RUN
#undef GETDT
#undef CE
#undef CTH
#undef BIND
#undef POSITION