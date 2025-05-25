/*
	Name: CompilerExceptionMessage.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 08/03/25 12:46
	Description: 此文件用于存放编译器异常信息的生成函数
*/

#pragma once

#include "String.hpp"

namespace stamon::c::err {
// 这里存放着编译端的报错信息生成函数

/*以下报错信息用于词法分析器*/

// 错误的字符串格式
String WrongStringFormat() {
	return String("the string was entered incorrectly");
}

// 错误的浮点数格式
String WrongFloatFormat() {
	return String("the floating point was entered incorrectly");
}

// 未知单词
String UnknownToken(String text) {
	return String("unknown token: \'") + text + String("\'");
}

/*以下报错信息用于语法分析器*/

// 错误语法
String InvalidSyntax() {
	return String("invalid syntax");
}

// 变量被重复定义
String VariableDeclaredRepeatedly(String iden) {
	return String("variable \"") + iden + String("\" are declared repeatedly");
}

// 错误的sfn语法
String WrongSfnSyntax() {
	return String(
			"the port or argument of the SFN statement must be an identifier");
}

// continue语句在循环外
String ContinueOutsideLoop() {
	return String("\'continue\' outside loop");
}

// break语句在循环外
String BreakOutsideLoop() {
	return String("\'break\' outside loop");
}

// 成员不是标识符
String WrongMemberFormat() {
	return String("the member name must be an identifier");
}

// 变量名不是标识符
String WrongVariableFormat() {
	return String("the name of the variable must be an identifier");
}

// 花括号未闭合
String BraceNotClosed() {
	return String("the brace are not closed");
}

// 方括号未闭合
String SquareBracketNotClosed() {
	return String("the square bracket are not closed");
}

// 圆括号未闭合
String RoundBracketNotClosed() {
	return String("the round bracket are not closed");
}

// 类定义中出现异常的成员定义
String WrongClassDefined() {
	return String("only functions, classes and variables "
				  "can be defined in a class");
}

// 无法引用其他源码
String CannotImport() {
	return String("cannot import");
}

// 需要左值作为赋值的左操作数
String LvalueRequieredLeftOperand() {
	return String("the left operand of an assignment must be an lvalue");
}

// 未定义的变量
String UndefinedVariable(String iden) {
	return String("undefined variable: \"") + iden + String("\"");
}

} // namespace stamon::c::err

namespace stamon::c::warning {
// 这里存放着编译端的警告信息生成函数

/*以下报错信息用于词法分析器*/

// 整数过大导致溢出
String TooLargeInteger() {
	return String("the integer is too large");
}

// 浮点数过大导致溢出
String TooLargeFloat() {
	return String("the floating point is too large");
}

} // namespace stamon::c::warning