/*
	Name: MemoryPool.hpp
	Copyright: Apache2.0
	Author: CLimber-Rong, copi143
	Date: 10/08/24 23:38
	Description: 内存池实现
*/

#pragma once

#include "ArrayList.hpp"
#include "Exception.hpp"
#include "NumberMap.hpp"
#include "TypeDef.hpp"

#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"

template<typename T> void *operator new(size_t size, T *ptr) {
	// 重载placement_new
	return (void *) ptr;
}

class MemoryPool {
	// 内存池

	using byte = bytes_get<1, false>::type;
	// 定义字节类型

	NumberMap<ArrayList<byte *>> free_mem_map;
	// 以空闲内存大小为键，空闲内存列表为值
	NumberMap<void> free_mems;
	// 以空闲内存大小为键和值
	// 用于快速得知free_mem_map的值有哪些
	int FreeMemConsumeSize = 0;
	// 目前空闲内存总大小为多少
	int PoolCacheSize;
	// 内存池缓存大小，如果超过限制则把空闲内存全部释放
	STMException *ex;

public:
	MemoryPool() {
	}

	MemoryPool(STMException *e, int mem_limit, int pool_cache_size) {
		ex = e;
		PoolCacheSize = pool_cache_size;
	}

	template<typename T, typename... Types> T *NewObject(Types &&...args) {
		if (PoolCacheSize == 0) {
			// 无缓存，直接使用new
			return new T(args...);
		} else {
			// 使用内存池
			byte *ptr;

			if (free_mem_map.containsKey(sizeof(T))) {
				ArrayList<byte *> *freelist = free_mem_map.get(sizeof(T));
				if (!freelist->empty()) {
					// 有空闲内存
					ptr = freelist->at(freelist->size() - 1);
					freelist->erase(freelist->size() - 1);
					FreeMemConsumeSize -= sizeof(T);

					new ((T *) ptr) T(args...);
					return (T *) ptr;
				}
			}

			ptr = (byte *) malloc(sizeof(T));

			new ((T *) ptr) T(args...);
			return (T *) ptr;
		}
	}

	void ClearAllFreeMem() {
		ArrayList<void *> frees = free_mems.getValList<void *>();

		for (int i = 0, len = frees.size(); i < len; i++) {
			// 遍历所有空闲内存大小
			ArrayList<byte *> *list = free_mem_map.get((size_t) (frees[i]));
			while (!list->empty()) {
				// 清空列表中所有元素
				free(list->at(list->size() - 1));
				list->erase(list->size() - 1);
				FreeMemConsumeSize -= (size_t) frees[i];
			}
		}

		return;
	}

	template<typename T> void DeleteObject(T *object) {
		if (PoolCacheSize == 0) {
			// 无缓存，直接使用delete
			delete object;
			return;
		} else {
			object->~T();

			byte *ptr = (byte *) object;

			if (PoolCacheSize == 0) {
				// 直接释放
				free(ptr);
				return;
			}

			if (FreeMemConsumeSize + sizeof(T) > PoolCacheSize) {
				// 释放所有空闲内存
				ClearAllFreeMem();
			}

			if (!free_mem_map.containsKey(sizeof(T))) {
				// 新建ArrayList
				free_mem_map.put(sizeof(T), new ArrayList<byte *>());
			}

			free_mem_map.get(sizeof(T))->add(ptr);
			free_mems.put(sizeof(T), (void *) sizeof(T));

			FreeMemConsumeSize += sizeof(T);

			return;
		}
	}
};