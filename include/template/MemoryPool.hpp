/*
	Name: MemoryPool.hpp
	Copyright: Apache2.0
	Author: CLimber-Rong, copi143
	Date: 10/08/24 23:38
	Description: 内存池实现
*/

#pragma once

#include "Exception.hpp"

class MemoryPool {
	// 内存池

public:

	MemoryPool();

	MemoryPool(STMException *e, int free_mem_limit, int pool_cache_size);

	template<typename T, typename... Types> T *NewObject(Types &&...args);

	template<typename T> void DeleteObject(T *object);
};