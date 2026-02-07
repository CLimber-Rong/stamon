/*
	Name: CompilerException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 07/08/2025 20:00
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::compiler {
inline log::Info StringError(String position) {
	return log::Info(String("compiler"), String("StringError"), String("the string was entered incorrectly"), position);
}

inline log::Info FloatError(String position) {
	return log::Info(String("compiler"), String("FloatError"), String("the floating point was entered incorrectly"), position);
}

inline log::Info TokenError(String position, String text) {
	return log::Info(String("compiler"), String("TokenError"), String("unknown token \'")+text+String("\'"), position);
}

inline log::Info SyntaxError(String position) {
	return log::Info(String("compiler"), String("SyntaxError"), String("invalid syntax"), position);
}

inline log::Info VariableRedeclaredError(String position, String iden) {
	return log::Info(String("compiler"), String("VariableRedeclaredError"), String("variable '")+iden+String("' are declared repeatedly"), position);
}

inline log::Info SfnError(String position) {
	return log::Info(String("compiler"), String("SfnError"), String("the port or argument of the SFN statement must be an identifier"), position);
}

inline log::Info BreakError(String position) {
	return log::Info(String("compiler"), String("BreakError"), String("\'break\' outside loop"), position);
}

inline log::Info ContinueError(String position) {
	return log::Info(String("compiler"), String("ContinueError"), String("\'continue\' outside loop"), position);
}

inline log::Info MemberError(String position) {
	return log::Info(String("compiler"), String("MemberError"), String("the member name must be an identifier"), position);
}

inline log::Info VariableError(String position) {
	return log::Info(String("compiler"), String("VariableError"), String("the name of the variable must be an identifier"), position);
}

inline log::Info BraceError(String position) {
	return log::Info(String("compiler"), String("BraceError"), String("the brace are not closed"), position);
}

inline log::Info SquareBracketError(String position) {
	return log::Info(String("compiler"), String("SquareBracketError"), String("the brace are not closed"), position);
}

inline log::Info RoundBracketError(String position) {
	return log::Info(String("compiler"), String("RoundBracketError"), String("the round bracket are not closed"), position);
}

inline log::Info ClassDefinedError(String position) {
	return log::Info(String("compiler"), String("ClassDefinedError"), String("only functions, classes and variables can be defined in a class"), position);
}

inline log::Info ImportError(String position) {
	return log::Info(String("compiler"), String("ImportError"), String("cannot import"), position);
}

inline log::Info AssignmentError(String position) {
	return log::Info(String("compiler"), String("AssignmentError"), String("the left operand of an assignment must be an lvalue"), position);
}

inline log::Info UndefinedVariableError(String position, String iden) {
	return log::Info(String("compiler"), String("UndefinedVariableError"), String("undefined variable \'")+iden+String("\'"), position);
}

inline log::Info LargeIntegerWarning(String position) {
	return log::Info(String("compiler"), String("LargeIntegerWarning"), String("the integer is too large"), position);
}

inline log::Info LargeFloatWarning(String position) {
	return log::Info(String("compiler"), String("LargeFloatWarning"), String("the floating point is too large"), position);
}

}