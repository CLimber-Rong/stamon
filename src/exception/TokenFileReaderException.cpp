/*
	Name: TokenFileReaderException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 22/07/2025 16:37
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::tokenfilereader {
inline STMInfo FormatError(String position) {
	return STMInfo(String("tokenfilereader"), String("FormatError"), String("non-standardized token-file"), position);
}

inline STMInfo TokenError(String position) {
	return STMInfo(String("tokenfilereader"), String("TokenError"), String("unknown token"), position);
}

}