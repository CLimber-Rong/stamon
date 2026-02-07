/*
	Name: TokenFileReaderException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 07/08/2025 20:00
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::tokenfilereader {
inline log::Info FormatError(String position) {
	return log::Info(String("tokenfilereader"), String("FormatError"), String("non-standardized token-file"), position);
}

inline log::Info TokenError(String position) {
	return log::Info(String("tokenfilereader"), String("TokenError"), String("unknown token"), position);
}

}