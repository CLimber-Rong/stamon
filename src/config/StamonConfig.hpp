/*
	Name: StamonConfig.hpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 08/03/25 18:21
	Description: 用于存储stamon的公共配置
*/

#pragma once

// stamon的版本号
namespace stamon {
constexpr int STAMON_VER_X = 2;
constexpr int STAMON_VER_Y = 4;
constexpr int STAMON_VER_Z = 44;
} // namespace stamon

namespace stamon::config {

enum STAMON_WARNING_SAFE_LEVEL {
	// 警告等级
	StamonWarningSafeLevel_IgnoreWarning = 0, // 忽略警告
	StamonWarningSafeLevel_JustWarn, // 只警告，程序继续运行
	StamonWarningSafeLevel_FatalWarning // 将警告视为致命错误
};

}

#include "CompilerConfig.hpp"
#include "VmConfig.hpp"