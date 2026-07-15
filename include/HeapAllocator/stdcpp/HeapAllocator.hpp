/*
	Name: HeapAllocator.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 07/06/26 21:01
	Description: 堆分配器
*/
#pragma once

#include "Allocator.hpp"
#include "StamonLib.hpp"
#include <cstdio>
#include <cstdlib>

namespace stamon::stdcpp {
class HeapAllocator : public Allocator {
public:
	virtual void *allocMem(Size size) override {
		void *ptr = std::malloc(size);
		if (!ptr) {
			perror("no more heap memory");
		}
		return ptr;
	}

	virtual void deallocMem(void *ptr) override {
		return std::free(ptr);
	}

	virtual ~HeapAllocator() = default;
};
} // namespace stamon::stdcpp

namespace stamon {
using HeapAllocator = stdcpp::HeapAllocator;
} // namespace stamon