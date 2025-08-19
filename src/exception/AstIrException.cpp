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
inline STMInfo RootNodeError(String position) {
	return STMInfo(String("astir"), String("RootNodeError"), String("expect ast-ir root node"), position);
}

inline STMInfo RedundantEndNodeError(String position) {
	return STMInfo(String("astir"), String("RedundantEndNodeError"), String("redundant ast-ir end node"), position);
}

inline STMInfo RedundantRootNodeError(String position) {
	return STMInfo(String("astir"), String("RedundantRootNodeError"), String("redundant ast-ir end node"), position);
}

inline STMInfo EndNodeError(String position) {
	return STMInfo(String("astir"), String("EndNodeError"), String("expect ast-ir end node"), position);
}

}