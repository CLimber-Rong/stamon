/*
	Name: TokenFileReader.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 14/12/24 23:22
	Description: 词法单元文件阅读器
*/

#pragma once

#include "BinaryReader.hpp"
#include "Exception.hpp"
#include "Lexer.cpp"
#include "String.hpp"

// 为了节约篇幅，定义了一些宏用于简写
// 这些宏只用于此文件
#define CE \
	CATCH { \
		return NULL; \
	}

namespace stamon::action {
class TokenFileReader {
	BinaryReader reader;
	char *buffer;
	int index;
	int lineno; // 当前行数
	int buffer_size;
	bool ismore = true;
	STMException *ex;

public:
	TokenFileReader() {
	}
	TokenFileReader(STMException *e, String filename) {
		ex = e;
		reader = BinaryReader(ex, filename);
		CATCH {
			return;
		}

		buffer = reader.read(); // 读取
		index = 0; // 当前阅读到第index字节
		lineno = 1; // 当前读取到第lineno行
		buffer_size = reader.getsize();

		if (buffer_size < 2 || buffer[0] != (char) 0xAB
				|| buffer[1] != (char) 0xDC) {
			// 如果文件过小或魔数异常则报错
			THROW_S(String("non-standardized token-file"));
			return;
		}
		buffer += 2; // 从buffer[2]开始读
	}

	char readbyte() {
		if (index >= buffer_size) {
			THROW_S(String("non-standardized token-file"));
			return 0;
		}
		char rst = buffer[index];
		index++;
		return rst;
	}

	c::Token *readToken() {
		// 获取一个Token
		int id = readbyte();

		if(id<0 || id>c::TokenNum) {
			THROW_S(String("unknown token"));
			return NULL;
		}

		if (id == -1) {
			// 换行
			lineno++;
		}

		if (id == c::TokenInt) {
			// 特判整数token
			int val;

			char *ptr = (char *) &val;

			ptr[0] = readbyte();
			ptr[1] = readbyte();
			ptr[2] = readbyte();
			ptr[3] = readbyte();

			CE;
			return (c::Token *) (new c::IntToken(lineno, val));
		}

		if (id == c::TokenDouble) {
			// 特判浮点token
			double val;

			char *ptr = (char *) &val;

			ptr[0] = readbyte();
			ptr[1] = readbyte();
			ptr[2] = readbyte();
			ptr[3] = readbyte();
			ptr[4] = readbyte();
			ptr[5] = readbyte();
			ptr[6] = readbyte();
			ptr[7] = readbyte();

			CE;
			return (c::Token *) (new c::DoubleToken(lineno, val));
		}

		if (id == c::TokenString) {
			// 特判字符串token
			// 先读取出字符串长度
			int len = (readbyte() << 24) + (readbyte() << 16) + (readbyte() << 8)
					+ (readbyte());
			CE;
			// 再读取到char数组里
			char *cstr = new char[len + 1];
			for (int i = 0; i < len; i++) {
				cstr[i] = readbyte();
			}
			CE;
			String val(cstr, len);
			delete cstr; // 释放内存
			return (c::Token *) (new c::StringToken(lineno, val));
		}

		if (id == c::TokenIden) {
			// 特判标识符token，其实现与字符串token类似
			// 先读取出标识符长度
			int len = (readbyte() << 24) + (readbyte() << 16) + (readbyte() << 8)
					+ (readbyte());
			CE;
			// 再读取到char数组里
			char *cstr = new char[len + 1];
			for (int i = 0; i < len; i++) {
				cstr[i] = readbyte();
			}
			CE;
			String val(cstr, len);
			delete cstr; // 释放内存
			return (c::Token *) (new c::IdenToken(lineno, val));
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