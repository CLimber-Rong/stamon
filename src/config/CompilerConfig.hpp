/*
	Name: CompilerConfig.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 08/03/25 18:19
	Description: 此文件用于存储编译器的相关配置信息
*/

#pragma once

#include "StamonConfig.hpp"
#include "String.hpp"

namespace stamon::c::config {

// 警告的重要程度
int WarningLevel = stamon::config::StamonWarningSafeLevel_JustWarn;
// 默认的目标文件名
String DefaultObjectFileName = String("a.stvc");
// 是否支持import
bool isSupportImport = true;
// 是否剥削调试信息
bool isStrip = false;

} // namespace stamon::c::config