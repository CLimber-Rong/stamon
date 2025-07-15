/*
	Name: AstFileReaderException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 13/07/2025 17:43
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::astfilereader {
inline STMInfo FormatError(String position) {
	return STMInfo(String("astfilereader"), String("FormatError"), String("non-standardized ast-file"), position);
}

inline STMInfo NodeError(String position) {
	return STMInfo(String("astfilereader"), String("NodeError"), String("unknown ast-node"), position);
}

inline STMInfo RedundantRootNodeError(String position) {
	return STMInfo(String("astfilereader"), String("RedundantRootNodeError"), String("redundant ast root node"), position);
}

inline STMInfo RootNodeError(String position) {
	return STMInfo(String("astfilereader"), String("RootNodeError"), String("expect ast root node"), position);
}

inline STMInfo RedundantEndNodeError(String position) {
	return STMInfo(String("astfilereader"), String("RedundantEndNodeError"), String("redundant ast end node"), position);
}

inline STMInfo EndNodeError(String position) {
	return STMInfo(String("astfilereader"), String("EndNodeError"), String("expect ast end node"), position);
}

}