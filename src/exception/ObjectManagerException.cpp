/*
	Name: ObjectManagerException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 13/07/2025 17:43
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::objectmanager {
inline STMInfo MemoryError(String position) {
	return STMInfo(String("objectmanager"), String("MemoryError"), String("out of memory"), position);
}

inline STMInfo PhysicalMemoryError(String position) {
	return STMInfo(String("objectmanager"), String("PhysicalMemoryError"), String("out of physical memory"), position);
}

inline STMInfo IdentifierError(String position) {
	return STMInfo(String("objectmanager"), String("IdentifierError"), String("undefined identifier"), position);
}

inline STMInfo ObjectError(String position) {
	return STMInfo(String("objectmanager"), String("ObjectError"), String("unknown object"), position);
}

}