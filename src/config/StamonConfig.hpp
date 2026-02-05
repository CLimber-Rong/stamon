/*
	Name: StamonConfig.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 08/03/25 18:21
	Description: 用于存储stamon的公共配置
*/

#pragma once

// stamon的版本号
namespace stamon::config {
constexpr int STAMON_VER_X = 2;
constexpr int STAMON_VER_Y = 4;
constexpr int STAMON_VER_Z = 58;
} // namespace stamon

namespace stamon::config {

enum STAMON_CODING_ENDIAN {
	// 机器的大小端控制
	StamonCodingLittleEndian = 0,
	StamonCodingBigEndian
};

enum STAMON_WARNING_SAFE_LEVEL {
	// 警告等级
	StamonWarningSafeLevel_IgnoreWarning = 0, // 忽略警告
	StamonWarningSafeLevel_JustWarn, // 只警告，程序继续运行
	StamonWarningSafeLevel_FatalWarning // 将警告视为致命错误
};

// 编码的端序
constexpr int StamonEncodingEndian = StamonCodingLittleEndian;
// 解码的顺序
constexpr int StamonDecodingEndian = StamonCodingLittleEndian;

} // namespace stamon::config

#include "CompilerConfig.hpp"
#include "VmConfig.hpp"