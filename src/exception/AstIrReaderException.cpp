/*
	Name: AstIrReaderException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 22/07/2025 16:37
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::astirreader {
inline STMInfo CodeSizeError(String position) {
	return STMInfo(String("astirreader"), String("CodeSizeError"), String("the code size is too small"), position);
}

inline STMInfo FormatError(String position) {
	return STMInfo(String("astirreader"), String("FormatError"), String("not stvc"), position);
}

inline STMInfo ConstantsError(String position) {
	return STMInfo(String("astirreader"), String("ConstantsError"), String("unknown constants"), position);
}

}