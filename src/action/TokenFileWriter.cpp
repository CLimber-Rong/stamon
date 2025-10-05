/*
	Name: TokenFileWriter.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 28/12/24 17:01
	Description: 词法单元编码文件写入器
*/

#pragma once

#include "BufferStream.cpp"
#include "Exception.hpp"
#include "String.hpp"
#include "Token.cpp"

namespace stamon::action {
class TokenFileWriter {
	BufferOutStream& stream;

public:
	STMException *ex;

	TokenFileWriter() {
	}

	TokenFileWriter(STMException *e, BufferOutStream &outstream)
		: ex(e)
		, stream(outstream) {
		stream.writeArray((byte) 0xAB);
		stream.write((byte) 0xDC);
	}

	void writeToken(c::Token *tok) {
		byte id = tok->type;

		stream.write(id);

		// 接着特判带有词法单元数据的词法单元

		switch (id) {
		case c::TokenInt: {
			// 特判整数token
			stream.write(((c::IntToken *) tok)->val);
			break;
		}

		case c::TokenDouble: {
			// 特判浮点token
			stream.write(((c::DoubleToken *) tok)->val);
			break;
		}

		case c::TokenString: {
			String val = ((c::StringToken *) tok)->val;

			stream.write(val.length());

			for (int index = 0; index < val.length(); index++) {
				stream.write(val[index]);
			}
			break;
		}

		case c::TokenIden: {
			// 特判标识符token，其实现与字符串token类似

			String iden = ((c::IdenToken *) tok)->iden;

			stream.write(iden.length());

			for (int index = 0; index < iden.length(); index++) {
				stream.write(iden[index]);
			}
			break;
		}

		default: {
			break;
		}
		}
	}

	void writeLineTokens(ArrayList<c::Token *> tokens) {
		// 写入一行的Tokens

		for (int i = 0, len = tokens.size(); i < len; i++) {
			writeToken(tokens[i]);
		}

		stream.write((byte) -1); // 写入EOL
	}

	void writeEOF() {
		stream.write(c::TokenEOF);
	}
};
} // namespace stamon::action