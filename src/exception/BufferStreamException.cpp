/*
	Name: BufferStreamException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 07/08/2025 20:00
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::bufferstream {
inline log::Info DataSizeError(String position) {
	return log::Info(String("bufferstream"), String("DataSizeError"), String("the size of data is too small"), position);
}

}