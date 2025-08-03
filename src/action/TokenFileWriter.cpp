/*
	Name: TokenFileWriter.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 28/12/24 17:01
	Description: 词法单元编码文件写入器
*/

#pragma once

#include "BinaryWriter.hpp"
#include "Exception.hpp"
#include "Token.cpp"
#include "String.hpp"

// 为了节约篇幅，定义了一些宏用于简写
// 这些宏只用于此文件

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
class TokenFileWriter {
	BinaryWriter writer;

public:
	STMException *ex;

	TokenFileWriter() {
	}

	TokenFileWriter(STMException *e, String filename) {
		ex = e;
		writer = BinaryWriter(ex, filename);
		WRITE(0xAB);
		WRITE(0xDC);
	}

	void writeToken(c::Token *tok) {
		int id = tok->type;

		WRITE(id);

		// 接着特判带有词法单元数据的词法单元
		if (id == c::TokenInt) {
			// 特判整数token
			WRITE_I(((c::IntToken *) tok)->val);
		}

		if (id == c::TokenDouble) {
			// 特判浮点token
			double val = ((c::DoubleToken *) tok)->val;

			char *ptr = (char *) &val;

			WRITE_I(*((int *) ptr));
			WRITE_I(*((int *) (ptr + 4)));
		}

		if (id == c::TokenString) {
			String val = ((c::StringToken *) tok)->val;

			WRITE_I(val.length());

			for (int index = 0; index < val.length(); index++) {
				WRITE(val[index]);
			}
		}

		if (id == c::TokenIden) {
			// 特判标识符token，其实现与字符串token类似

			String iden = ((c::IdenToken *) tok)->iden;

			WRITE_I(iden.length());

			for (int index = 0; index < iden.length(); index++) {
				WRITE(iden[index]);
			}
		}
	}

	void writeLineTokens(ArrayList<c::Token *> tokens) {
		// 写入一行的Tokens

		for (int i = 0, len = tokens.size(); i < len; i++) {
			writeToken(tokens[i]);
			CATCH {
				return;
			}
		}

		WRITE(-1); // 写入EOL

		return;
	}

	void close() {
		WRITE(c::TokenEOF);

		writer.close();
		return;
	}
};
} // namespace stamon::action

#undef WRITE
#undef WRITE_I