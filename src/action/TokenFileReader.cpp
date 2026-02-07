/*
	Name: TokenFileReader.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 14/12/24 23:22
	Description: 词法单元文件阅读器
*/

#pragma once

#include "BufferStream.cpp"
#include "String.hpp"
#include "Token.cpp"
#include "TokenFileReaderException.cpp"

// 为了节约篇幅，定义了一些宏用于简写
// 这些宏只用于此文件
#define CE \
	CATCH { \
		return NULL; \
	}

namespace stamon::action {
class TokenFileReader {
	BufferInStream stream;
	int lineno; // 当前行数
	bool ismore; // 是否读完
	log::Exception *ex;

public:
	TokenFileReader() {
	}
	TokenFileReader(log::Exception *e, const BufferInStream &instream)
		: ex(e)
		, stream(instream)
		, ismore(true) {
		byte magic_number[2];
		stream.readArray(magic_number);

		CATCH {
			return;
		}

		if (magic_number[0] != (byte) 0xAB || magic_number[1] != (byte) 0xDC) {
			// 如果文件过小或魔数异常则报错
			THROW(exception::tokenfilereader::FormatError("TokenFileReader()"));
			return;
		}
	}

	c::Token *readToken() {
		// 获取一个Token
		byte id;
		stream.read(id);
		CE;

		if (id < 0 || id > c::TokenNum) {
			THROW(exception::tokenfilereader::TokenError("readToken()"));
			return NULL;
		}

		switch (id) {
		case (byte)-1: {
			lineno++;
			break;
		}

		case c::TokenInt: {
			// 特判整数token
			int val;
			stream.read(val);
			CE;

			return (c::Token *) (new c::IntToken(lineno, val));
		}

		case c::TokenDouble: {
			// 特判浮点token
			double val;
			stream.read(val);
			CE;

			return (c::Token *) (new c::DoubleToken(lineno, val));
		}

		case c::TokenString: {
			// 特判字符串token
			int len;
			stream.read(len);
			CE;
			if (len < 0) {
				THROW(exception::tokenfilereader::FormatError("readToken()"));
			}

			// 再读取到byte数组里
			byte *cstr = new byte[len + 1];
			cstr[len] = '\0';
			for (int i = 0; i < len; i++) {
				stream.read(cstr[i]);
				CE;
			}
			String val(cstr, len);
			delete cstr; // 释放内存
			return (c::Token *) (new c::StringToken(lineno, val));
		}

		case c::TokenIden: {
			// 特判标识符token
			int len;
			stream.read(len);
			CE;
			if (len < 0) {
				THROW(exception::tokenfilereader::FormatError("readToken()"));
			}

			// 再读取到byte数组里
			byte *cstr = new byte[len + 1];
			cstr[len] = '\0';
			for (int i = 0; i < len; i++) {
				stream.read(cstr[i]);
				CE;
			}
			String val(cstr, len);
			delete cstr; // 释放内存
			return (c::Token *) (new c::StringToken(lineno, val));
		}

		default: {
			break;
		}
		}

		// 返回正常的Token
		return (c::Token *) (new c::Token(lineno, id));
	}

	ArrayList<c::Token *> readLineTokens() {
		/*
		 * 获取一行的Token
		 * 在调用此方法之前，请先调用isMore()来确保还有剩余的Token
		 */

		ArrayList<c::Token *> result;

		c::Token *token = readToken();

		CATCH {
			return result;
		}

		while (token->type != -1 && token->type != c::TokenEOF) {
			result.add(token);
			token = readToken();
		}

		if (token->type == c::TokenEOF) {
			ismore = false;
		}

		return result;
	}

	bool isMore() {
		return ismore;
	}

	void close() {
		reader.close();
		return;
	}
};
} // namespace stamon::action

#undef CE