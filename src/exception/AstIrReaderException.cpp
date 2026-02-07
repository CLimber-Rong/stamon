/*
	Name: AstIrReaderException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 07/08/2025 20:00
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::astirreader {
inline log::Info FormatError(String position) {
	return log::Info(String("astirreader"), String("FormatError"), String("not stvc"), position);
}

inline log::Info NodeError(String position) {
	return log::Info(String("astirreader"), String("NodeError"), String("unknown ast-ir node"), position);
}

}