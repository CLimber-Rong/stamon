/*
	Name: AstRunnerException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 22/07/2025 16:37
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::astrunner {
inline STMInfo TypeError(String position, String type) {
	return STMInfo(String("astrunner"), String("TypeError"), String("an error of data type \'")+type+String("' occurred in the calculation"), position);
}

inline STMInfo PostfixError(String position) {
	return STMInfo(String("astrunner"), String("PostfixError"), String("unknown type of postfix"), position);
}

inline STMInfo IndexError(String position) {
	return STMInfo(String("astrunner"), String("IndexError"), String("list index out of range"), position);
}

inline STMInfo ConstantsError(String position) {
	return STMInfo(String("astrunner"), String("ConstantsError"), String("wrong index of constants"), position);
}

inline STMInfo ZeroDivisionError(String position) {
	return STMInfo(String("astrunner"), String("ZeroDivisionError"), String("division by zerodivision by zero"), position);
}

inline STMInfo BreakError(String position) {
	return STMInfo(String("astrunner"), String("BreakError"), String("\'break\' outside loop"), position);
}

inline STMInfo ContinueError(String position) {
	return STMInfo(String("astrunner"), String("ContinueError"), String("\'continue\' outside loop"), position);
}

inline STMInfo ArgumentsError(String position, String form_args, String actual_args) {
	return STMInfo(String("astrunner"), String("ArgumentsError"), String("takes ")+form_args+String("  form arguments but ")+actual_args+String(" was given"), position);
}

inline STMInfo ReturnError(String position) {
	return STMInfo(String("astrunner"), String("ReturnError"), String("\'return\' outside function"), position);
}

inline STMInfo UnknownOperatorError(String position) {
	return STMInfo(String("astrunner"), String("UnknownOperatorError"), String("unknown operator"), position);
}

inline STMInfo UnknownMemberError(String position, String id) {
	return STMInfo(String("astrunner"), String("UnknownMemberError"), String("object has no member \'")+id+String("\'"), position);
}

inline STMInfo LengthError(String position) {
	return STMInfo(String("astrunner"), String("LengthError"), String("the length must be non-negative"), position);
}

inline STMInfo NegativeShiftError(String position) {
	return STMInfo(String("astrunner"), String("NegativeShiftError"), String("negative shift count"), position);
}

}