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
inline STMInfo ConstantsError(String position) {
	return STMInfo(String("consttabreader"), String("ConstantsError"), String("unknown constants"), position);
}

inline STMInfo FormatError(String position) {
	return STMInfo(String("consttabreader"), String("FormatError"), String("not stvc"), position);
}

}