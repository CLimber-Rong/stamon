/*
	Name: ConstTabReader.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 05/08/25 21:08
	Description: 常量表解码器
*/

#pragma once

#include "ArrayList.hpp"
#include "BufferStream.cpp"
#include "ConstTabReaderException.cpp"
#include "DataType.hpp"
#include "String.hpp"

// 用于简写的宏
#define CE \
	CATCH { \
		return; \
	}

namespace stamon::action {
class ConstTabReader {
	BufferInStream stream;

public:
	log::Exception *ex;
	ArrayList<datatype::DataType *> tableConst; // 常量表

	ConstTabReader(log::Exception *e, const BufferInStream &instream)
		: ex(e)
		, stream(instream) {
	}

	void read() {
		// 读取常量表，size为常量表项数，pos为下一个读取的字节的下标
		int size;
		stream.read(size);

		if (size < 0) {
			THROW(exception::consttabreader::FormatError("read()"));
			return;
		}

		for (int i = 1; i <= size; i++) {
			byte type;
			stream.read(type);
			CE;

			switch (type) {
			case datatype::IntegerTypeID: {
				int value;
				stream.read(value);
				CE;
				tableConst.add(new datatype::IntegerType(value));
				break;
			}

			case datatype::FloatTypeID: {
				float value;
				stream.read(value);
				CE;
				tableConst.add(new datatype::FloatType(value));
				break;
			}

			case datatype::DoubleTypeID: {
				double value;
				stream.read(value);
				CE;
				tableConst.add(new datatype::DoubleType(value));
				break;
			}

			case datatype::NullTypeID: {
				tableConst.add(new datatype::NullType());
				break;
			}

			case datatype::StringTypeID: {
				int len;
				stream.read(len);
				CE;

				if (len < 0) {
					THROW(exception::consttabreader::FormatError("read()"));
					return;
				}

				byte *cstr = new byte[len + 1];
				cstr[len] = '\0';

				for (int i = 0; i < len; i++) {
					stream.read(cstr[i]);
					CE;
				}

				String s(cstr);
				delete[] cstr;

				tableConst.add(new datatype::StringType(s));
				break;
			}

			case (byte) ir::IdenConstTypeID: {
				int len;
				stream.read(len);
				CE;

				if (len < 0) {
					THROW(exception::consttabreader::FormatError("read()"));
					return;
				}

				byte *cstr = new byte[len + 1];
				cstr[len] = '\0';

				for (int i = 0; i < len; i++) {
					stream.read(cstr[i]);
					CE;
				}

				String s(cstr);
				delete[] cstr;

				tableConst.add(new ir::IdenConstType(s));
				break;
			}

			default: {
				THROW(exception::consttabreader::ConstantsError("read()"));
				return;
			}
			}
		}
	}
};
} // namespace stamon::action