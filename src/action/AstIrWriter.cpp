/*
	Name: AstIrWriter.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 09/12/23 17:34
	Description: IR写入器
*/

#pragma once

#include "AstIr.cpp"
#include "BinaryWriter.hpp"
#include "DataType.hpp"
#include "Exception.hpp"

#define WRITE(b) \
	writer.write(b); \
	CATCH { \
		return; \
	}

#define WRITE_I(n) \
	writer.write_i(n); \
	CATCH { \
		return; \
	}

namespace stamon::action {
class AstIrWriter {
	BinaryWriter writer;

public:
	STMException *ex;

	AstIrWriter() {
	}
	AstIrWriter(STMException *e, String filename) {
		ex = e;

		writer = BinaryWriter(ex, filename);

		CATCH {
			return;
		}

		// 写入魔数

		WRITE(0xAB);
		WRITE(0xDB);
	}

	void write(ArrayList<ir::AstIr> ir_list,
			ArrayList<datatype::DataType *> ir_tableconst, bool isStrip,
			int VerX, int VerY, int VerZ) {
		// 写入版本

		WRITE(VerX)
		WRITE(VerY)
		WRITE(VerZ)

		// 写入常量表

		WRITE_I(ir_tableconst.size())

		for (int i = 0, len = ir_tableconst.size(); i < len; i++) {
			WRITE((char) ir_tableconst[i]->getType())

			if (ir_tableconst[i]->getType() == datatype::IntegerTypeID) {
				int val = ((datatype::IntegerType *) ir_tableconst[i])->getVal();

				WRITE_I(val);
			}

			if (ir_tableconst[i]->getType() == datatype::FloatTypeID) {
				float val = ((datatype::FloatType *) ir_tableconst[i])->getVal();
				// 逐个字节写入
				char *val_ptr = (char *) &val;
				WRITE(val_ptr[0]);
				WRITE(val_ptr[1]);
				WRITE(val_ptr[2]);
				WRITE(val_ptr[3]);
			}

			if (ir_tableconst[i]->getType() == datatype::DoubleTypeID) {
				double val = ((datatype::DoubleType *) ir_tableconst[i])->getVal();
				// 逐个字节写入
				char *val_ptr = (char *) &val;
				WRITE(val_ptr[0]);
				WRITE(val_ptr[1]);
				WRITE(val_ptr[2]);
				WRITE(val_ptr[3]);
				WRITE(val_ptr[4]);
				WRITE(val_ptr[5]);
				WRITE(val_ptr[6]);
				WRITE(val_ptr[7]);
			}

			if (ir_tableconst[i]->getType() == datatype::StringTypeID) {
				String s = ((datatype::StringType *) ir_tableconst[i])->getVal();

				WRITE_I(s.length());

				for (int i = 0, len = s.length(); i < len; i++) {
					WRITE(s[i]);
				}
			}

			if (ir_tableconst[i]->getType() == ir::IdenConstTypeID) {
				String s = ((ir::IdenConstType *) ir_tableconst[i])->getVal();

				WRITE_I(s.length());

				for (int i = 0, len = s.length(); i < len; i++) {
					WRITE(s[i])
				}
			}
		}

		// 最后写入IR

		int lineNo = -1; // 当前正在写入的行号
		String filename((char *) ""); // 当前正在写入的文件名

		for (int i = 0, len = ir_list.size(); i < len; i++) {
			if (isStrip == false) {
				// 要求附加调试信息

				if (lineNo != ir_list[i].lineNo && ir_list[i].type != -1) {
					// 注意：结束符不用在意行号和文件名
					// 这里要特判结束符（即ir_list[i].type!=-1）

					// 如果行号没更新，更新行号并且将当前行号写入
					lineNo = ir_list[i].lineNo;

					WRITE_I(-2); //-2代表更新行号
					WRITE_I(lineNo);
				}

				if (filename.equals(ir_list[i].filename) == false
						&& ir_list[i].type != -1) {
					// 检查文件名，原理同上
					filename = ir_list[i].filename;

					WRITE_I(-3); //-2代表更新文件
					WRITE_I(filename.length());

					for (int i = 0, len = filename.length(); i < len; i++) {
						WRITE(filename[i]);
					}
				}
			}

			WRITE_I(ir_list[i].type);
			WRITE_I(ir_list[i].data);
		}
	}

	void close() {
		writer.close();
	}
};
} // namespace stamon::action

#undef WRITE
#undef WRITE_I