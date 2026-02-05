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
#include "BasicIo.hpp"
#include "BasicPlatform.hpp"
#include "Compiler.hpp"
#include "DataType.hpp"
#include "Exception.hpp"
#include "ObjectManager.cpp"
#include "StamonConfig.hpp"
#include "StamonLib.hpp"
#include "String.hpp"

// 用于简写的宏
#define CE \
	CATCH { \
		ErrorMsg->add(ex->getError().toString()); \
		return; \
	}

namespace stamon {

class Stamon {
public:
	EasySmartPtr<STMException> ex;
	EasySmartPtr<ArrayList<String>> ErrorMsg;
	EasySmartPtr<ArrayList<String>> WarningMsg;

	int VerX, VerY, VerZ; // 这三个变量代表版本为X.Y.Z

	Stamon()
		: ex(new STMException)
		, ErrorMsg(new ArrayList<String>())
		, WarningMsg(new ArrayList<String>())
		, VerX(config::STAMON_VER_X)
		, VerY(config::STAMON_VER_Y)
		, VerZ(config::STAMON_VER_Z) {
	}

	void compile(String src, String dst, bool isSupportImport, bool isStrip) {
		ArrayList<c::SourceSyntax> *syntax_list = c::ParseTargetProject(
				ex.get(), ErrorMsg.get(), WarningMsg.get(), src,
				isSupportImport, new ArrayList<c::SourceSyntax>(),
				HashMap<String, bool>(), c::SyntaxScope(ex.get()));

		// 开始编译

		if (!ErrorMsg->empty()) {
			// 出现报错
			return;
		}

		// 将编译结果整合成一个AST

		ast::AstNode *node = c::MergeAST(syntax_list);

		// 编译为IR

		ir::AstIrConverter converter(ex.get());

		ArrayList<ir::AstIr> ir_list = converter.ast2ir(node);
		// AST转为AST-IR

		CE;

		delete node; // 删除AST

		// 开始写入

		ArrayList<datatype::DataType *> ir_tableconst = converter.tableConst;

		action::BufferOutStream stream(ex.get());
		CE;

		action::AstIrWriter writer(ex.get(), stream);
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

		action::BufferInStream stream(ex.get(), src);

		CE;

		action::AstIrReader ir_reader(ex.get(), stream);
		// 初始化字节码读取器

		CE;

		ir_reader.readHeader();
		CE;
		// 读取文件头

		const ArrayList<ir::AstIr>& ir_list = ir_reader.readIR();
		// 读取IR

		VerX = ir_reader.VerX;
		VerY = ir_reader.VerY;
		VerZ = ir_reader.VerZ;
		// 复制版本号

		ir::AstIrConverter converter(ex.get());

		converter.tableConst = move(ir_reader.tableConst);
		// 移动常量表到转换器

		ast::AstNode *running_node = converter.ir2ast(ir_list);
		// 复原成AST

		CE;

		vm::AstRunner runner;

		runner.execute(running_node, isGC, MemLimit, converter.tableConst,
				ArrayList<String>(), PoolCacheSize, ex.get());
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

		action::BufferInStream instream(ex.get(), src);
		// 输入流

		action::AstIrReader ir_reader(ex.get(), instream);
		// 初始化字节码读取器
		CE;

		ir_reader.readHeader();
		CE;
		// 读取文件头

		ir_list = ir_reader.readIR();
		CE;
		// 读取IR

		action::BufferOutStream outstream(ex.get());
		// 输出流

		action::AstIrWriter writer(ex.get(), outstream);

		writer.write(ir_list, ir_reader.tableConst, true, VerX, VerY, VerZ);

		outstream.writeFile(src);
		CE;
		// 写入文件

		ir::AstIrConverter converter(ex.get());

		converter.destroyConst(ir_reader.tableConst);
		// 利用转换器来销毁常量表
		CE;

		for (int i = 0, len = ex->getWarning().size(); i < len; i++) {
			WarningMsg->add(ex->getWarning().at(i).toString());
		}

		return;
	}

	EasySmartPtr<ArrayList<String>> getErrorMessages() {
		return ErrorMsg;
	}

	EasySmartPtr<ArrayList<String>> getWarningMessages() {
		return WarningMsg;
	}
};
} // namespace stamon

#undef CE