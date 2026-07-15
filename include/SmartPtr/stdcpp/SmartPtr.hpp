/*
	Name: SmartPtr.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 12/07/26 15:25
	Description: 智能指针的标准C++实现
*/

#pragma once

#include "Allocator.hpp"
#include "AllocatorAdapter.hpp"
#include "GlobalConfig.hpp"
#include "StamonLib.hpp"
#include "SmartPtrTrait.hpp"
#include <memory>

namespace stamon::stdcpp {
template<typename T> class SmartPtr {
	// 注意：只用于托管单对象，托管数组请使用ArrayList封装

	class Deallocator {
		Allocator *deallocator;

	public:
		Deallocator(Allocator *deallocator)
			: deallocator(deallocator) {
		}
		void operator()(T *ptr) {
			deallocator->dealloc(ptr);
		}
	};

	std::shared_ptr<T> ptr;
	// 注意：只用于托管单对象，托管数组请使用ArrayList封装

public:
	SmartPtr(T *ptr, Allocator *allocator = &config::DefaultAllocator,
			Allocator *deallocator = &config::DefaultAllocator)
		: ptr(ptr, Deallocator(deallocator), AllocatorAdapter<T>(allocator)) {
	}

	T *get() const {
		return ptr.get();
	}

	T *operator->() const {
		return ptr.get();
	}

	T &operator*() const {
		return *ptr;
	}
};
} // namespace stamon::stdcpp

namespace stamon {
template<typename T>
using SmartPtr = trait::checkSmartPtrTrait<stdcpp::SmartPtr<T>, T>;
}