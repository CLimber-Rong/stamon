/*
	Name: AstIrWriter.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 09/12/23 17:34
	Description: IR写入器
*/

#pragma once

#include "AstIr.cpp"
#include "BufferStream.cpp"
#include "ConstTabWriter.cpp"
#include "DataType.hpp"

namespace stamon::action {
class AstIrWriter {
	BufferOutStream& stream;

public:
	STMException *ex;

	AstIrWriter(STMException *e, BufferOutStream &outstream)
		: ex(e)
		, stream(outstream) {
		stream.write((byte) 0xAB);
		stream.write((byte) 0xDB);
	}

	void write(ArrayList<ir::AstIr> ir_list,
			ArrayList<datatype::DataType *> ir_tableconst, bool isStrip,
			int VerX, int VerY, int VerZ) {
		// 写入版本

		stream.write(VerX);
		stream.write(VerY);
		stream.write(VerZ);

		// 写入常量表

		ConstTabWriter tabwriter(ex, stream);
		tabwriter.write(ir_tableconst);

		// 最后写入IR

		int lineNo = -1; // 当前正在写入的行号
		String filename; // 当前正在写入的文件名

		for (int i = 0, len = ir_list.size(); i < len; i++) {
			if (isStrip == false) {
				// 要求附加调试信息

				if (lineNo != ir_list[i].lineNo && ir_list[i].type != -1) {
					// 注意：结束符不用在意行号和文件名
					// 这里要特判结束符（即ir_list[i].type!=-1）

					// 如果行号没更新，更新行号并且将当前行号写入
					lineNo = ir_list[i].lineNo;

					stream.write(-2); //-2代表更新行号
					stream.write(lineNo);
				}

				if (filename.equals(ir_list[i].filename) == false
						&& ir_list[i].type != -1) {
					// 检查文件名，原理同上
					filename = ir_list[i].filename;

					stream.write(-3); //-2代表更新文件
					stream.write(filename.length());

					for (int i = 0, len = filename.length(); i < len; i++) {
						stream.write(filename[i]);
					}
				}
			}

			stream.write(ir_list[i].type);
			stream.write(ir_list[i].data);
		}
	}
};
} // namespace stamon::action