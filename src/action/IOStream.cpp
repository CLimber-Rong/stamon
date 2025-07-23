/*
	Name: IOStream.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 23/07/25 14:54
	Description: 输入输出流的实现，用于行动工具中编解码器的数据读写
*/

#pragma once

#include "ArrayList.hpp"
#include "Exception.hpp"

namespace stamon::action {
/*
 * 由于不能直接依赖C++标准库，因此这里简单的实现一个IStream和OStream
 */
class IStream {
	//输入流的基类
public:
	STMException *ex;

	IStream(STMException *e)
		: ex(e) {
	}

	virtual void read(char *data, int size) = 0;
	virtual int getsize() = 0;

	template<typename T> void read(T &data) {
		// 类型转换辅助
		read((char *) &data, sizeof(T));
	}
};

class OStream {
	//输出流的基类
public:
	STMException *ex;

	OStream(STMException *e)
		: ex(e) {
	}

	virtual void write(const char *data, int size) = 0;
	virtual IStream toIStream() = 0; // 转换为IStream，用于数据传递

	template<typename T> void write(const T &data) {
		// 类型转换辅助
		write((char *) &data, sizeof(T));
	}
}
} // namespace stamon::action