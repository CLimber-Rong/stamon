/*
	Name: Stamon.hpp
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
#include"AstIr.cpp"
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
			STMException *ex;
			ArrayList<String> ErrorMsg;
			ArrayList<String> WarningMsg;

			int VerX, VerY, VerZ;   //这三个变量代表版本为X.Y.Z

			Stamon() {}

			void Init() {
				ex = new STMException();
				VerX = STAMON_VER_X;
				VerY = STAMON_VER_Y;
				VerZ = STAMON_VER_Z;
			}

			void compile(
			    String src, String dst, bool isSupportImport, bool isStrip
			) {
				c::Compiler compiler(ex, &ErrorMsg, &WarningMsg);

				compiler.compile(src, isSupportImport);
				//开始编译

				if(ErrorMsg.empty() == false) {
					//出现报错
					return;
				}

				//将编译结果整合成一个AST

				ArrayList<ast::AstNode*>* program 
										  = new ArrayList<ast::AstNode*>();

				for(int i=0,len=compiler.src->size(); i<len; i++) {
					//获取该文件的AST根节点
					ast::AstNode* node = compiler.src->at(i).program;

					//放入总节点中
					(*program) += *(node->Children());

					//node只是暂存了ast，需要被删除
					//为了防止删除时将子节点也删除，因此需要先清空字节点列表
					node->Children()->clear();
					delete node;
				}

				ast::AstNode* node = new ast::AstProgram(program);

				//初始化node
				node->filename = String();
				node->lineNo = -1;

				//编译为IR

				ir::AstIrConverter converter;

				ArrayList<ir::AstIr> ir_list = converter.ast2ir(node);

				delete node;	//删除AST

				//开始写入

				ArrayList<datatype::DataType*>
				ir_tableconst = converter.tableConst;

				action::AstIrWriter writer(ex, dst);

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				writer.write(
					ir_list, ir_tableconst, isStrip,
					VerX, VerY, VerZ
				);

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				converter.destroyConst(converter.tableConst);

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				return;
			}

			void run(String src, bool isGC, int MemLimit, int PoolCacheSize) {

				//实现流程：文件读取器->字节码读取器->IR读取器->虚拟机

				ArrayList<ir::AstIr> ir_list;

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				action::AstIrReader ir_reader(ex, src);
				//初始化字节码读取器

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				if(ir_reader.ReadHeader()==false) {
					//读取文件头
					ErrorMsg.add(ex->getError());
					return;
				}

				ir_list = ir_reader.ReadIR();

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				ir_reader.close();

				//复制版本号
				VerX = ir_reader.VerX;
				VerY = ir_reader.VerY;
				VerZ = ir_reader.VerZ;

				ir::AstIrConverter converter;	//初始化转换器

				converter.tableConst = ir_reader.tableConst;
				//复制常量表到转换器

				ast::AstNode* running_node = converter.ir2ast(ir_list);

				vm::AstRunner runner;

				runner.execute(
				    running_node, isGC, MemLimit, converter.tableConst,
				    ArrayList<String>(), PoolCacheSize ,ex
				);

				delete running_node;	//删除AST

				converter.destroyConst(converter.tableConst);

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				WarningMsg = runner.ex->getWarning();

				return;
			}

			void strip(String src) {
				//剥夺调试信息
				//这些代码直接改编自run方法和compile方法

				ArrayList<ir::AstIr> ir_list;

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				action::AstIrReader ir_reader(ex, src);
				//初始化字节码读取器

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				if(ir_reader.ReadHeader()==false) {
					//读取文件头
					ErrorMsg.add(ex->getError());
					return;
				}

				ir_list = ir_reader.ReadIR();

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				ir_reader.close();	//关闭文件

				ArrayList<datatype::DataType*>
				ir_tableconst = ir_reader.tableConst;

				//写入魔数
				
				action::AstIrWriter writer(ex, src);

				writer.write(
					ir_list, ir_tableconst, true,
					VerX, VerY, VerZ
				);

				ir::AstIrConverter converter;
				//利用转换器来销毁常量表
				converter.destroyConst(ir_tableconst);

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				WarningMsg = ex->getWarning();

				return;
			}

			ArrayList<String> getErrorMessages() {
				return ErrorMsg;
			}

			ArrayList<String> getWarningMessages() {
				return WarningMsg;
			}

			~Stamon() {
				delete ex;
			}
	};
} //namespace stamon