/*
	Name: ConstTabWriter.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 05/08/25 15:01
	Description: 常量表编码器
*/

#pragma once

#include "AstIr.cpp"
#include "BufferStream.cpp"
#include "DataType.hpp"
#include "Exception.hpp"
#include "String.hpp"

namespace stamon::action {
class ConstTabWriter {
	BufferOutStream &stream;

public:
	STMException *ex;

	ConstTabWriter(STMException *e, BufferOutStream &outstream)
		: ex(e)
		, stream(outstream) {
	}

	void write(ArrayList<datatype::DataType *> tableconst) {
		stream.write(tableconst.size());

		for (int i = 0, len = tableconst.size(); i < len; i++) {
			stream.write((char) tableconst[i]->getType());

			switch (tableconst[i]->getType()) {
			case datatype::IntegerTypeID: {
				int val = ((datatype::IntegerType *) tableconst[i])->getVal();
				stream.write(val);
				break;
			}

			case datatype::FloatTypeID: {
				float val = ((datatype::FloatType *) tableconst[i])->getVal();
				stream.write(val);
				break;
			}

			case datatype::DoubleTypeID: {
				double val = ((datatype::DoubleType *) tableconst[i])->getVal();
				stream.write(val);
				break;
			}

			case datatype::StringTypeID: {
				String s = ((datatype::StringType *) tableconst[i])->getVal();
				stream.write(s.length());
				for (int i = 0, len = s.length(); i < len; i++) {
					stream.write(s[i]);
				}
				break;
			}

			case ir::IdenConstTypeID: {
				String s = ((ir::IdenConstType *) tableconst[i])->getVal();
				stream.write(s.length());
				for (int i = 0, len = s.length(); i < len; i++) {
					stream.write(s[i]);
				}
				break;
			}

			default: {
				break;
			}
			}
		}
	}
};
} // namespace stamon::action