/*
	Name: VmConfig.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 08/03/25 18:35
	Description: 此文件用于存储虚拟机的相关配置信息
*/

#pragma once

#include "StamonConfig.hpp"

namespace stamon::vm::config {

// 警告的重要程度
int WarningLevel = stamon::config::StamonWarningSafeLevel_JustWarn;
// 是否启用垃圾回收
bool isGC = true;
// 虚拟机对象占用的内存限制
int MemLimit = 16 * 1024 * 1024;
// 内存池的缓存大小
int PoolCacheSize = 16 * 1024 * 1024;

} // namespace stamon::vm::config