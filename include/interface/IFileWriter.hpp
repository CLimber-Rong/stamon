/*
	Name: IFileWriter.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 03/10/25 16:19
	Description: 文件写入类接口
*/

#pragma once

#include "BasicPlatform.hpp"
#include "Exception.hpp"
#include "StamonLib.hpp"
#include "String.hpp"

namespace stamon {

template<class Impl> class IFileWriter : public Impl {
	//文件写入器，引用传递
public:
	IFileWriter(STMException *e, String filename)
		: Impl(e, move(filename)) {
		// 打开名为filename的文件
	}
	void write(byte b) {
		// 写入一个字节
		return Impl::write(move(b));
	}
	void close() {
		// 关闭文件
		return Impl::close();
	}
};

} // namespace stamon