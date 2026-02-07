/*
	Name: AstRunnerException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 07/08/2025 20:00
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::astrunner {
inline log::Info TypeError(String position, String type) {
	return log::Info(String("astrunner"), String("TypeError"), String("an error of data type \'")+type+String("' occurred in the calculation"), position);
}

inline log::Info PostfixError(String position) {
	return log::Info(String("astrunner"), String("PostfixError"), String("unknown type of postfix"), position);
}

inline log::Info IndexError(String position) {
	return log::Info(String("astrunner"), String("IndexError"), String("list index out of range"), position);
}

inline log::Info ConstantsError(String position) {
	return log::Info(String("astrunner"), String("ConstantsError"), String("wrong index of constants"), position);
}

inline log::Info ZeroDivisionError(String position) {
	return log::Info(String("astrunner"), String("ZeroDivisionError"), String("division by zerodivision by zero"), position);
}

inline log::Info BreakError(String position) {
	return log::Info(String("astrunner"), String("BreakError"), String("\'break\' outside loop"), position);
}

inline log::Info ContinueError(String position) {
	return log::Info(String("astrunner"), String("ContinueError"), String("\'continue\' outside loop"), position);
}

inline log::Info ArgumentsError(String position, String form_args, String actual_args) {
	return log::Info(String("astrunner"), String("ArgumentsError"), String("takes ")+form_args+String("  form arguments but ")+actual_args+String(" was given"), position);
}

inline log::Info ReturnError(String position) {
	return log::Info(String("astrunner"), String("ReturnError"), String("\'return\' outside function"), position);
}

inline log::Info UnknownOperatorError(String position) {
	return log::Info(String("astrunner"), String("UnknownOperatorError"), String("unknown operator"), position);
}

inline log::Info UnknownMemberError(String position, String id) {
	return log::Info(String("astrunner"), String("UnknownMemberError"), String("object has no member \'")+id+String("\'"), position);
}

inline log::Info LengthError(String position) {
	return log::Info(String("astrunner"), String("LengthError"), String("the length must be non-negative"), position);
}

inline log::Info NegativeShiftError(String position) {
	return log::Info(String("astrunner"), String("NegativeShiftError"), String("negative shift count"), position);
}

}