/*
	Name: AstFileReaderException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 07/08/2025 20:00
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::astfilereader {
inline log::Info FormatError(String position) {
	return log::Info(String("astfilereader"), String("FormatError"), String("non-standardized ast-file"), position);
}

inline log::Info NodeError(String position) {
	return log::Info(String("astfilereader"), String("NodeError"), String("unknown ast-node"), position);
}

inline log::Info RedundantRootNodeError(String position) {
	return log::Info(String("astfilereader"), String("RedundantRootNodeError"), String("redundant ast root node"), position);
}

inline log::Info RootNodeError(String position) {
	return log::Info(String("astfilereader"), String("RootNodeError"), String("expect ast root node"), position);
}

inline log::Info RedundantEndNodeError(String position) {
	return log::Info(String("astfilereader"), String("RedundantEndNodeError"), String("redundant ast end node"), position);
}

inline log::Info EndNodeError(String position) {
	return log::Info(String("astfilereader"), String("EndNodeError"), String("expect ast end node"), position);
}

}