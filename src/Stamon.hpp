/*
	Name: Stamon.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 24/12/23 11:23
	Description: Stamon头文件
*/

#pragma once

#include "ArrayList.hpp"
#include "Ast.hpp"
#include "AstIr.cpp"
#include "AstIrReader.cpp"
#include "AstIrWriter.cpp"
#include "AstRunner.cpp"
#include "Compiler.hpp"
#include "DataType.hpp"
#include "Exception.hpp"
#include "ObjectManager.cpp"
#include "StamonConfig.hpp"
#include "String.hpp"

// 用于简写的宏
#define CE \
	CATCH { \
		ErrorMsg.add(ex->getError().toString()); \
		return; \
	}

namespace stamon {

class Stamon {
public:
	STMException *ex;
	ArrayList<String> ErrorMsg;
	ArrayList<String> WarningMsg;

	int VerX, VerY, VerZ; // 这三个变量代表版本为X.Y.Z

	Stamon() {
	}

	void init() {
		ex = new STMException();
		VerX = STAMON_VER_X;
		VerY = STAMON_VER_Y;
		VerZ = STAMON_VER_Z;
	}

	void compile(String src, String dst, bool isSupportImport, bool isStrip) {
		ArrayList<c::SourceSyntax> *syntax_list = c::ParseTargetProject(ex,
				&ErrorMsg, &WarningMsg, src, isSupportImport,
				new ArrayList<c::SourceSyntax>(), StringMap<void>(),
				c::SyntaxScope(ex));

		// 开始编译

		if (ErrorMsg.empty() == false) {
			// 出现报错
			return;
		}

		// 将编译结果整合成一个AST

		ast::AstNode *node = c::MergeAST(syntax_list);

		// 编译为IR

		ir::AstIrConverter converter(ex);

		ArrayList<ir::AstIr> ir_list = converter.ast2ir(node);
		// AST转为AST-IR

		CE;

		delete node; // 删除AST

		// 开始写入

		ArrayList<datatype::DataType *> ir_tableconst = converter.tableConst;

		action::BufferOutStream stream(ex);
		CE;

		action::AstIrWriter writer(ex, stream);
		CE;

		writer.write(ir_list, ir_tableconst, isStrip, VerX, VerY, VerZ);
		CE;

		stream.writeFile(dst);
		CE;
		// 写入文件

		converter.destroyConst(converter.tableConst);
		CE;
		// 利用转换器来销毁常量表

		return;
	}

	void run(String src, bool isGC, int MemLimit, int PoolCacheSize) {
		// 实现流程：文件读取器->字节码读取器->IR读取器->虚拟机

		ArrayList<ir::AstIr> ir_list;

		CE;

		action::BufferInStream stream(ex, src);

		action::AstIrReader ir_reader(ex, stream);
		// 初始化字节码读取器

		CE;

		ir_reader.readHeader();
		CE;
		// 读取文件头

		ir_list = ir_reader.readIR();
		CE;
		// 读取IR

		VerX = ir_reader.VerX;
		VerY = ir_reader.VerY;
		VerZ = ir_reader.VerZ;
		// 复制版本号

		ir::AstIrConverter converter(ex);

		converter.tableConst = ir_reader.tableConst;
		// 复制常量表到转换器

		ast::AstNode *running_node = converter.ir2ast(ir_list);
		// 复原成AST

		CE;

		vm::AstRunner runner;

		runner.execute(running_node, isGC, MemLimit, converter.tableConst,
				ArrayList<String>(), PoolCacheSize, ex);
		// 执行

		delete running_node;
		// 删除AST

		converter.destroyConst(converter.tableConst);
		CE;
		// 利用转换器来销毁常量表

		ArrayList<STMInfo> runner_warning = runner.ex->getWarning();
		CE;

		return;
	}

	void strip(String src) {
		// 剥夺调试信息
		// 这些代码直接改编自run方法和compile方法

		ArrayList<ir::AstIr> ir_list;

		action::BufferInStream instream(ex, src);
		// 输入流

		action::AstIrReader ir_reader(ex, instream);
		// 初始化字节码读取器
		CE;

		ir_reader.readHeader();
		CE;
		// 读取文件头

		ir_list = ir_reader.readIR();
		CE;
		// 读取IR

		action::BufferOutStream outstream(ex);
		// 输出流

		action::AstIrWriter writer(ex, outstream);

		writer.write(ir_list, ir_reader.tableConst, true, VerX, VerY, VerZ);

		outstream.writeFile(src);
		CE;
		// 写入文件

		ir::AstIrConverter converter(ex);

		converter.destroyConst(ir_reader.tableConst);
		// 利用转换器来销毁常量表
		CE;

		for (int i = 0, len = ex->getWarning().size(); i < len; i++) {
			WarningMsg.add(ex->getWarning().at(i).toString());
		}

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
} // namespace stamon

#undef CE