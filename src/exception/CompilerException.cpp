/*
	Name: CompilerException.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 22/07/2025 16:37
	Description: 报错信息函数，由codegen.py自动生成
*/

#pragma once
#include "String.hpp"
#include "Exception.hpp"

namespace stamon::exception::compiler {
inline STMInfo StringError(String position) {
	return STMInfo(String("compiler"), String("StringError"), String("the string was entered incorrectly"), position);
}

inline STMInfo FloatError(String position) {
	return STMInfo(String("compiler"), String("FloatError"), String("the floating point was entered incorrectly"), position);
}

inline STMInfo TokenError(String position, String text) {
	return STMInfo(String("compiler"), String("TokenError"), String("unknown token \'")+text+String("\'"), position);
}

inline STMInfo SyntaxError(String position) {
	return STMInfo(String("compiler"), String("SyntaxError"), String("invalid syntax"), position);
}

inline STMInfo VariableRedeclaredError(String position, String iden) {
	return STMInfo(String("compiler"), String("VariableRedeclaredError"), String("variable '")+iden+String("' are declared repeatedly"), position);
}

inline STMInfo SfnError(String position) {
	return STMInfo(String("compiler"), String("SfnError"), String("the port or argument of the SFN statement must be an identifier"), position);
}

inline STMInfo BreakError(String position) {
	return STMInfo(String("compiler"), String("BreakError"), String("\'break\' outside loop"), position);
}

inline STMInfo ContinueError(String position) {
	return STMInfo(String("compiler"), String("ContinueError"), String("\'continue\' outside loop"), position);
}

inline STMInfo MemberError(String position) {
	return STMInfo(String("compiler"), String("MemberError"), String("the member name must be an identifier"), position);
}

inline STMInfo VariableError(String position) {
	return STMInfo(String("compiler"), String("VariableError"), String("the name of the variable must be an identifier"), position);
}

inline STMInfo BraceError(String position) {
	return STMInfo(String("compiler"), String("BraceError"), String("the brace are not closed"), position);
}

inline STMInfo SquareBracketError(String position) {
	return STMInfo(String("compiler"), String("SquareBracketError"), String("the brace are not closed"), position);
}

inline STMInfo RoundBracketError(String position) {
	return STMInfo(String("compiler"), String("RoundBracketError"), String("the round bracket are not closed"), position);
}

inline STMInfo ClassDefinedError(String position) {
	return STMInfo(String("compiler"), String("ClassDefinedError"), String("only functions, classes and variables can be defined in a class"), position);
}

inline STMInfo ImportError(String position) {
	return STMInfo(String("compiler"), String("ImportError"), String("cannot import"), position);
}

inline STMInfo AssignmentError(String position) {
	return STMInfo(String("compiler"), String("AssignmentError"), String("the left operand of an assignment must be an lvalue"), position);
}

inline STMInfo UndefinedVariableError(String position, String iden) {
	return STMInfo(String("compiler"), String("UndefinedVariableError"), String("undefined variable \'")+iden+String("\'"), position);
}

inline STMInfo LargeIntegerWarning(String position) {
	return STMInfo(String("compiler"), String("LargeIntegerWarning"), String("the integer is too large"), position);
}

inline STMInfo LargeFloatWarning(String position) {
	return STMInfo(String("compiler"), String("LargeFloatWarning"), String("the floating point is too large"), position);
}

}