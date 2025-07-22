/*
	Name: Token.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 16/07/25 14:19
	Description: 用于存储Token的数据结构
*/

#pragma once

#include "String.hpp"

namespace stamon::c {
enum TOKEN_TYPE {
	TokenClass = 0,
	TokenDef,
	TokenExtends,
	TokenFunc,
	TokenBreak,
	TokenContinue,
	TokenIf,
	TokenElse,
	TokenWhile,
	TokenFor,
	TokenIn,
	TokenReturn,
	TokenSFN,
	TokenNew,
	TokenNull, // 空值
	TokenImport, // 导入
	TokenExtern,	//外部定义
	KEYWORDS_MAX, // 关键词个数
	TokenAssign, // 赋值
	TokenSemi, // 分号
	TokenLBC, // 左花括号（Left BraCe）
	TokenRBC, // 右花括号（Right BraCe）
	TokenLRB, // 左括号（Left Round Bracket）
	TokenRRB, // 右括号（Right Round Bracket）
	TokenLSB, // 左方括号（Left Square Bracket）
	TokenRSB, // 右方括号（RightSquare Bracket）
	TokenCmm, // 逗号
	TokenColon, // 冒号
	TokenMember, // 小数点
	TokenAddAss, // 加等于
	TokenSubAss,
	TokenMulAss,
	TokenDivAss,
	TokenModAss,
	TokenAndAss,
	TokenXOrAss,
	TokenOrAss,
	TokenLSHAss,
	TokenRSHAss,
	MATH_OPERATOR_START,	//数学运算符的起始
	TokenLogOR, // 逻辑运算符
	TokenLogAND,
	TokenBitOR, // 位运算符
	TokenBitXOR,
	TokenBitAND,
	TokenEqu,
	TokenNotEqu,
	TokenBig,
	TokenLess,
	TokenBigEqu,
	TokenLessEqu,
	TokenLSH, // 左移
	TokenRSH, // 右移
	TokenAdd,
	TokenSub,
	TokenMul,
	TokenDiv,
	TokenMod,
	TokenLogNot, // 逻辑非
	TokenBitNot, // 按位取反
	TokenIden, // 标识符
	TokenInt, // 整数
	TokenDouble, // 浮点数
	TokenString, // 字符串
	TokenTrue, // 布尔真值
	TokenFalse, // 布尔假值
	TokenEOF,
	TokenNum
};

class Token {
public:
	Token(int line, int tok_type) {
		lineNo = line;
		type = tok_type;
	}
	int type;
	int lineNo;
};

class StringToken : public Token {
public:
	String val;
	StringToken(int line, const String &s)
		: Token(line, TokenString) {
		val = s;
	}
};

class IdenToken : public Token {
public:
	String iden;
	IdenToken(int line, const String &name)
		: Token(line, TokenIden) {
		iden = name;
	}
};

class IntToken : public Token {
public:
	int val;
	IntToken(int line, int value)
		: Token(line, TokenInt) {
		val = value;
	}
};

class DoubleToken : public Token {
public:
	double val;
	DoubleToken(int line, double value)
		: Token(line, TokenDouble) {
		val = value;
	}
};

Token TokEOF(-1, TokenEOF);
} // namespace stamon::c