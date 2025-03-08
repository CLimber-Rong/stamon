/*
	Name: Stamon.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 24/12/23 11:23
	Description: Stamon头文件
*/

#pragma once

#include"ArrayList.hpp"
#include"String.hpp"
#include"DataType.hpp"
#include"AstRunner.cpp"
#include"ObjectManager.cpp"
#include"Ast.hpp"
#include"AstIrReader.cpp"
#include"AstIrWriter.cpp"
#include"AstIR.cpp"
#include"Compiler.hpp"
#include"Exception.hpp"
#include"BinaryReader.hpp"
#include"StamonConfig.hpp"

namespace stamon {
	// using namespace stamon::ir;
	// using namespace stamon::datatype;
	// using namespace stamon::c;
	// using namespace stamon::vm;
	// using namespace stamon::sfn;

	class Stamon {
		public:
			STMException* ex;
			ArrayList<String>* ErrorMsg;
			ArrayList<String>* WarningMsg;

			int VerX, VerY, VerZ;   //这三个变量代表版本为X.Y.Z

			Stamon() {}

			void Init() {
				ex = new STMException();
				ErrorMsg = new ArrayList<String>();
				WarningMsg = new ArrayList<String>();
				VerX = STAMON_VER_X;
				VerY = STAMON_VER_Y;
				VerZ = STAMON_VER_Z;
			}

			void compile(
			    String src, String dst, bool isSupportImport, bool isStrip
			) {
				c::Compiler compiler(ex);

				compiler.compile(src, isSupportImport);	//开始编译

				WarningMsg = compiler.WarningMsg;	//获取警告信息

				if(compiler.ErrorMsg->empty()==false) {
					ErrorMsg = compiler.ErrorMsg;
					return;
				}

				//将编译结果整合成一个AST

				ArrayList<ast::AstNode*> program;

				for(int i=0,len=compiler.src->size(); i<len; i++) {
					ArrayList<ast::AstNode*> temp = *(
												compiler
												.src
												->at(i)
												.program
												->Children()
											);
					program += temp;
				}

				ast::AstNode* node = new ast::AstProgram(&program);
				
				//初始化node
				node->filename = String();
				node->lineNo = -1;

				//编译为IR

				ir::AstIRGenerator generator;

				ArrayList<ir::AstIR> ir_list = generator.gen(node);

				//开始写入

				ArrayList<datatype::DataType*>
				ir_tableconst = generator.tableConst;

				ir::AstIrWriter writer(ex);

				writer.write(
					ir_list, ir_tableconst, dst, isStrip,
					VerX, VerY, VerZ
				);

				CATCH {
					ErrorMsg->add(ex->getError());
					return;
				}

				return;
			}

			void run(String src, bool isGC, int MemLimit, int PoolCacheSize) {

				//实现流程：文件读取器->字节码读取器->IR读取器->虚拟机

				ArrayList<ir::AstIR> ir_list;

				BinaryReader reader(ex, src);	//打开文件

				CATCH {
					ErrorMsg->add(ex->getError());
					return;
				}

				ir::AstIrReader ir_reader(reader.read(), reader.size, ex);
				//初始化字节码读取器

				CATCH {
					ErrorMsg->add(ex->getError());
					return;
				}

				if(ir_reader.ReadHeader()==false) {
					//读取文件头
					ErrorMsg->add(ex->getError());
					return;
				}

				ir_list = ir_reader.ReadIR();

				CATCH {
					ErrorMsg->add(ex->getError());
					return;
				}

				reader.close();	//关闭文件

				//复制版本号
				VerX = ir_reader.VerX;
				VerY = ir_reader.VerY;
				VerZ = ir_reader.VerZ;

				ir::AstIRGenerator generator;	//初始化IR读取器

				generator.tableConst = ir_reader.tableConst;
				//复制常量表到IR读取器

				vm::AstRunner runner;

				ast::AstNode* running_node = generator.read(ir_list);

				runner.excute(
				    running_node, isGC, MemLimit, generator.tableConst,
				    ArrayList<String>(), PoolCacheSize ,ex
				);

				CATCH {
					ErrorMsg->add(ex->getError());
					return;
				}

				*WarningMsg = runner.ex->getWarning();

				return;
			}

			void strip(String src) {
				//剥夺调试信息
				//这些代码直接改编自run方法和compile方法

				ArrayList<ir::AstIR> ir_list;

				BinaryReader reader(ex, src);	//打开文件

				CATCH {
					ErrorMsg->add(ex->getError());
					return;
				}

				ir::AstIrReader ir_reader(reader.read(), reader.size, ex);
				//初始化字节码读取器

				CATCH {
					ErrorMsg->add(ex->getError());
					return;
				}

				if(ir_reader.ReadHeader()==false) {
					//读取文件头
					ErrorMsg->add(ex->getError());
					return;
				}

				ir_list = ir_reader.ReadIR();

				CATCH {
					ErrorMsg->add(ex->getError());
					return;
				}

				reader.close();	//关闭文件

				ArrayList<datatype::DataType*>
				ir_tableconst = ir_reader.tableConst;

				//写入魔数
				
				ir::AstIrWriter writer(ex);

				writer.write(
					ir_list, ir_tableconst, src, true,
					VerX, VerY, VerZ
				);

				CATCH {
					ErrorMsg->add(ex->getError());
					return;
				}

				*WarningMsg = ex->getWarning();

				return;
			}

			ArrayList<String>* getErrorMessages() {
				return ErrorMsg;
			}

			ArrayList<String>* getWarningMessages() {
				return WarningMsg;
			}

			~Stamon() {
				delete ex;
				delete ErrorMsg;
				delete WarningMsg;
			}
	};
} //namespace stamon