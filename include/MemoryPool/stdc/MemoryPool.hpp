/*
	Name: MemoryPool.hpp
	License: Apache2.0
	Author: CLimber-Rong, copi143
	Date: 10/08/24 23:38
	Description: 内存池实现
*/

#pragma once

#include "ArrayList.hpp"
#include "BasicPlatform.hpp"
#include "HashMap.hpp"
#include "IMemoryPool.hpp"

#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"

namespace stamon::stdc {

class MemoryPool {
	// 内存池

	HashMap<int, ArrayList<byte *> *> free_mem_map;
	// 以空闲内存大小为键，空闲内存列表为值
	int FreeMemConsumeSize = 0;
	// 目前空闲内存总大小为多少
	int PoolCacheSize;
	// 内存池缓存大小，如果超过限制则把空闲内存全部释放
	STMException *ex;

public:
	MemoryPool(STMException *e, int mem_limit, int pool_cache_size) {
		ex = e;
		PoolCacheSize = pool_cache_size;
	}

	template<typename T, typename... Types> T *NewObject(Types &&...args) {
		if (PoolCacheSize <= 0) {
			// 无缓存，直接使用new
			return new T(forward<Types>(args)...);
		} else {
			// 使用内存池
			byte *ptr;

			if (free_mem_map.exist(sizeof(T))) {
				ArrayList<byte *> *freelist = free_mem_map.get(sizeof(T));
				if (!freelist->empty()) {
					// 有空闲内存
					ptr = freelist->at(freelist->size() - 1);
					freelist->erase(freelist->size() - 1);
					FreeMemConsumeSize -= sizeof(T);

					new ((T *) ptr) T(forward<Types>(args)...);
					return (T *) ptr;
				}
			}

			ptr = (byte *) malloc(sizeof(T));

			new ((T *) ptr) T(forward<Types>(args)...);
			return (T *) ptr;
		}
	}

	void clearAllFreeMem() {
		ArrayList<int> free_mem_type = free_mem_map.getKeyList();
		// 获取空闲内存块有几种不同大小

		for (int i = 0, ilen = free_mem_type.size(); i < ilen; i++) {
			// 遍历所有空闲内存种类
			ArrayList<byte *> *list = free_mem_map.get(free_mem_type[i]);
			// 获取当前种类的空闲内存列表
			for (int j = 0, jlen = list->size(); j < jlen; j++) {
				free(list->at(j));
				// 释放空闲内存
			}
			FreeMemConsumeSize -= free_mem_type[i] * list->size();
			// 减去占用内存
			list->clear();
			// 清空列表
			delete list;
			// 删除列表
			free_mem_map.del(free_mem_type[i]);
			// 该类型无任何空闲内存
		}

		return;
	}

	template<typename T> void DeleteObject(T *object) {
		if (PoolCacheSize <= 0) {
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
				clearAllFreeMem();
			}

			if (!free_mem_map.exist(sizeof(T))) {
				// 新建ArrayList
				free_mem_map.put(sizeof(T), new ArrayList<byte *>());
			}

			free_mem_map.get(sizeof(T))->add(ptr);

			FreeMemConsumeSize += sizeof(T);

			return;
		}
	}
};

} // namespace stamon::stdc

namespace stamon {

using MemoryPool = interface::IMemoryPool<stdc::MemoryPool>;

}