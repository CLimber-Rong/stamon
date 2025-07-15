/*
	Name: SFNException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 13/07/2025 17:43
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::sfn {
inline STMInfo SFNError(String position, String msg) {
	return STMInfo(String("sfn"), String("SFNError"), msg, position);
}

inline STMInfo SFNWarning(String position, String msg) {
	return STMInfo(String("sfn"), String("SFNWarning"), msg, position);
}

}