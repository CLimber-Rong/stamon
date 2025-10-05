/*
	Name: Exception.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 24/08/23 18:09
	Description: 异常处理，该类无需特殊实现
*/
#pragma once

#include "ArrayList.hpp"
#include "StamonLib.hpp"
#include "BasicPlatform.hpp"
#include "String.hpp"

/*
	CATCH的使用方法：
	CATCH {
		//在这里写上捕捉异常的代码
	}
	THROW(info)用于抛出异常，异常信息为info
	WARN(info)用于抛出警告，警告信息为info
	ISWARNING用于判断是否有警告被抛出，是则为true，否则为false
*/

/*
 * 开发者只需要在当前代码所在的作用域中定义STMException* ex
 * 即可使用以下的宏
 */

#define THROW(info) ex->Throw(info);
#define CATCH if (ex->isError)

#define WARN(info) ex->Warn(info);
#define ISWARNING (ex->isWarning)

namespace stamon {

class STMInfo {
	// 一个信息的组成，通常用于异常和日志系统
public:
	String sender; // 信息发送者
	String type; // 信息类型
	String message; // 信息内容
	String position; // 信息位置
	STMInfo() {
	}
	STMInfo(const String &Sender, const String &Type, const String &Message,
			const String &Position)
		: sender(Sender)
		, type(Type)
		, message(Message)
		, position(Position) {
	}
	String toString() {
		return type + String(": from ") + sender + String(": at ") + position
			 + String(": ") + message;
	}
};

class STMException {
public:
	STMInfo Exception; // 异常
	ArrayList<STMInfo> Warning; // 一系列的警告
	bool isError = false;
	bool isWarning = false;

	void Throw(STMInfo info) {
		Exception = info;
		isError = true;
	}

	void Warn(STMInfo info) {
		Warning.add(info);
		isWarning = true;
	}

	STMInfo getError() {
		return Exception;
	}

	ArrayList<STMInfo> getWarning() {
		return Warning;
	}
};

} // namespace stamon