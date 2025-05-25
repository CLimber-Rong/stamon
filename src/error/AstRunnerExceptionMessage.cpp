/*
	Name: AstRunnerExceptionMessage.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 08/03/25 13:53
	Description: 此文件用于存放AstRunner的异常信息生成函数
*/

#pragma once

#include "AstIr.cpp"
#include "DataType.hpp"

namespace stamon::vm {

// 获取执行中的位置信息
inline String stamon::vm::AstRunner::getExecutePosition() {
	if (RunningFileName.equals("") && RunningLineNo == -1) {
		// 字节码中被没有调试信息，即被字节码被strip过
		return String("");
	} else {
		return String("at \"") + RunningFileName + String("\": ")
			 + toString(RunningLineNo) + String(": ");
	}
}

// 类型错误
inline void AstRunner::ThrowTypeError(int type) {
	THROW_S(String("Type Error: ") + getExecutePosition()
			+ String("an error of data type \'") + getDataTypeName(type)
			+ String("\' occurred in the calculation"))
}

// 未知的后缀
inline void AstRunner::ThrowPostfixError() {
	THROW_S(String("ast::Postfix Error: ") + getExecutePosition()
			+ String("unknown type of postfix"))
}

// 下标越界
inline void AstRunner::ThrowIndexError() {
	THROW_S(String("Index Error: ") + getExecutePosition()
			+ String("list index out of range"))
}

// 常量表下标错误
inline void AstRunner::ThrowConstantsError() {
	THROW_S(String("Constants Error: ") + getExecutePosition()
			+ String("wrong index of constants"))
}

// 除数为0
inline void AstRunner::ThrowDivZeroError() {
	THROW_S(String("Zero Division Error: ") + getExecutePosition()
			+ String("division by zero"))
}

// break语句在循环外
inline void AstRunner::ThrowBreakError() {
	THROW_S(String("Break Error: ") + getExecutePosition()
			+ String("\'break\' outside loop"))
}

// continue语句在循环外
inline void AstRunner::ThrowContinueError() {
	THROW_S(String("Continue Error: ") + getExecutePosition()
			+ String("\'continue\' outside loop"))
}

// 参数传递错误
inline void AstRunner::ThrowArgumentsError(int form_args, int actual_args) {
	THROW_S(String("Arguments Error: ") + getExecutePosition() + String("takes ")
			+ toString(form_args) + String(" form arguments but ")
			+ toString(actual_args) + String(" was given"))
}

// 返回语句在函数外
inline void AstRunner::ThrowReturnError() {
	THROW_S(String("Return Error: ") + getExecutePosition()
			+ String("\'return\' outside function"))
}

// 未知运算符
inline void AstRunner::ThrowUnknownOperatorError() {
	THROW_S(String("Operator Error: ") + getExecutePosition()
			+ String("unknown operator"))
}

// 未知成员
inline void AstRunner::ThrowUnknownMemberError(int id) {
	String iden = ((ir::IdenConstType *) tabconst[id])->getVal();
	THROW_S(String("Unknown Member Error: ") + getExecutePosition()
			+ String("object has no member \'") + iden + String("\'"))
}

// 长度不能为负数
inline void AstRunner::ThrowLengthError() {
	THROW_S(String("Length Error: ") + getExecutePosition()
			+ String("the length must be non-negative"))
}

// 位移运算操作数不能为负数
inline void AstRunner::ThrowNegativeShiftError() {
	THROW_S(String("Negative Shift Error: ") + getExecutePosition()
			+ String("negative shift count"))
}

} // namespace stamon::vm