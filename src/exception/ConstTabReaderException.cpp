/*
	Name: ConstTabReaderException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 07/08/2025 20:00
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::consttabreader {
inline log::Info ConstantsError(String position) {
	return log::Info(String("consttabreader"), String("ConstantsError"), String("unknown constants"), position);
}

inline log::Info FormatError(String position) {
	return log::Info(String("consttabreader"), String("FormatError"), String("not stvc"), position);
}

}