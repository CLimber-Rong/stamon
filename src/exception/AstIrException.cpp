/*
	Name: AstIrException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 07/08/2025 20:00
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::astir {
inline log::Info RootNodeError(String position) {
	return log::Info(String("astir"), String("RootNodeError"), String("expect ast-ir root node"), position);
}

inline log::Info RedundantEndNodeError(String position) {
	return log::Info(String("astir"), String("RedundantEndNodeError"), String("redundant ast-ir end node"), position);
}

inline log::Info RedundantRootNodeError(String position) {
	return log::Info(String("astir"), String("RedundantRootNodeError"), String("redundant ast-ir end node"), position);
}

inline log::Info EndNodeError(String position) {
	return log::Info(String("astir"), String("EndNodeError"), String("expect ast-ir end node"), position);
}

}