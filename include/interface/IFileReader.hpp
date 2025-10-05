/*
	Name: IFileReader.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 03/10/25 16:15
	Description: 文件读取类接口
*/

#pragma once

#include "BasicPlatform.hpp"
#include "EasySmartPtr.hpp"
#include "Exception.hpp"
#include "StamonLib.hpp"
#include "String.hpp"

namespace stamon {

ArrayList<String> ImportPaths;

template<class Impl> class IFileReader : public Impl {
	//文件读取器，引用传递
public:
	IFileReader(STMException *e, String filename)
		: Impl(e, move(filename)) {
		// 打开名为filename的文件
	}
	int getSize() {
		// 获取文件大小
		return Impl::getSize();
	}
	EasySmartPtr<byte> read() {
		// 读取文件为一个字节串
		return Impl::read();
	}
	void close() {
		// 关闭文件
		return Impl::close();
	}
};

} // namespace stamon