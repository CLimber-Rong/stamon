/*
	Name: GlobalConfig.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 08/03/25 18:21
	Description: 用于存储stamon的公共配置
*/

#pragma once

#include "HeapAllocator.hpp"

// stamon的版本号
namespace stamon::config {

struct StamonVersion {
	unsigned int X;
	unsigned int Y;
	unsigned int Z;
	constexpr StamonVersion(unsigned int X, unsigned int Y, unsigned int Z)
		: X(X)
		, Y(Y)
		, Z(Z) {
	}
};

constexpr StamonVersion Version(2,5,0);

} // namespace stamon::config

namespace stamon::config {

enum class CodingEndianEnum {
	Little = 0,
	Big
};

enum class SafeLevelEnum {
	IgnoreWarning = 0,
	JustWarn,
	FatalWarning
};

// 编码的端序
constexpr CodingEndianEnum EncodingEndian = CodingEndianEnum::Little;
// 解码的顺序
constexpr CodingEndianEnum DecodingEndian = CodingEndianEnum::Little;
//安全等级
constexpr SafeLevelEnum SafeLevel = SafeLevelEnum::JustWarn;

// 默认的分配器
HeapAllocator DefaultAllocator;

} // namespace stamon::config