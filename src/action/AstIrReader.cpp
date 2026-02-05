/*
	Name: AstIrReader.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 09/12/23 17:34
	Description: IR读取器
*/

#pragma once

#include "AstIr.cpp"
#include "AstIrReaderException.cpp"
#include "BufferStream.cpp"
#include "ConstTabReader.cpp"
#include "DataType.hpp"
#include "String.hpp"

// 用于简写的宏
#define CE \
	CATCH { \
		return; \
	}

namespace stamon::action {
class AstIrReader {
	BufferInStream stream;

public:
	int VerX, VerY, VerZ; // 这三个变量代表版本为X.Y.Z
	ArrayList<datatype::DataType *> tableConst; // 常量表

	STMException *ex;

	AstIrReader(STMException *e, const BufferInStream &instream)
		: ex(e)
		, stream(instream) {
	}

	void readHeader() {
		// 读取魔数
		byte magic_number[2];
		stream.readArray(magic_number);
		CE;

		if (magic_number[0] != (byte) 0xAB || magic_number[1] != (byte) 0xDB) {
			// 检查字节码是否为AST-IR
			printf("%d %d \n",magic_number[0],magic_number[1]);
			THROW(exception::astirreader::FormatError("readHeader()"));
		}

		// 读取版本号
		stream.read(VerX);
		CE;
		stream.read(VerY);
		CE;
		stream.read(VerZ);
		CE;

		// 读取常量表
		ConstTabReader reader(ex, stream);
		reader.read();
		CE;
		tableConst = move(reader.tableConst);
	}

	ArrayList<ir::AstIr> readIR() {
		ArrayList<ir::AstIr> ir;

		int lineNo = -1;

		String filename;

		while (stream.isMore()) {
			int type;
			stream.read(type);
			CATCH {
				return ir;
			}

			if (type == -2) {
				// 更新行号
				stream.read(lineNo);

			} else if (type == -3) {
				// 更新文件名
				int len;
				stream.read(len);
				CATCH {
					return ir;
				}

				if (len < 0) {
					THROW(exception::astirreader::FormatError("readIR()"));
					return ir;
				}

				byte *cstr = new byte[len + 1];
				cstr[len] = '\0';

				for (int i = 0; i < len; i++) {
					stream.read(cstr[i]);
				}

				filename = String(cstr);

				delete[] cstr;

			} else {
				// 正常的IR
				ir::AstIr rst;

				stream.read(rst.data);

				if (type < -1 || type >= ast::AstTypeNum) {
					//不属于正常AST编码，也不是结尾单元
					THROW(exception::astirreader::NodeError("readIR()"));
					return ir;
				}

				CATCH {
					return ir;
				}

				rst.lineNo = lineNo;
				rst.filename = filename;
				rst.type = type;

				ir.add(rst);
			}
		}

		return ir;
	}
};
} // namespace stamon::action

#undef CE