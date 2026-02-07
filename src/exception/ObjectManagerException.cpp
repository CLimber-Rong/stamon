/*
	Name: ObjectManagerException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 07/08/2025 20:00
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::objectmanager {
inline log::Info MemoryError(String position) {
	return log::Info(String("objectmanager"), String("MemoryError"), String("out of memory"), position);
}

inline log::Info PhysicalMemoryError(String position) {
	return log::Info(String("objectmanager"), String("PhysicalMemoryError"), String("out of physical memory"), position);
}

inline log::Info IdentifierError(String position) {
	return log::Info(String("objectmanager"), String("IdentifierError"), String("undefined identifier"), position);
}

inline log::Info ObjectError(String position) {
	return log::Info(String("objectmanager"), String("ObjectError"), String("unknown object"), position);
}

}