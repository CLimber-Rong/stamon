/*
	Name: Allocator.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 07/06/26 20:22
	Description: 分配器基类
*/

#pragma once

#include "StamonLib.hpp"

namespace stamon {
struct PlacementTag {}; // 占位符
} // namespace stamon

inline void *operator new(
		stamon::Size, stamon::PlacementTag, void *ptr) noexcept {
	return ptr;
}

// 用以应对构造函数抛出异常
inline void operator delete(void *, stamon::PlacementTag, void *) noexcept {
	// 无需释放内存
}

namespace stamon {

template<typename T, typename... Args>
T *PlacementNew(void *ptr, Args &&...args) {
	return new (PlacementTag(), ptr) T(forward<Args>(args)...);
}

template<typename T> void PlacementDelete(T *ptr) {
	ptr->~T();
}

class Allocator {
public:
	// 开辟或释放内存，面向内存地址
	virtual void *allocMem(Size) = 0;
	virtual void deallocMem(void *) = 0;

	// 申请或删除对象，面向对象
	template<typename T, typename... Args> T *alloc(Args &&...args) {
		return PlacementNew<T>(allocMem(sizeof(T)), forward<Args>(args)...);
	}

	template<typename T> void dealloc(T *ptr) {
		PlacementDelete(ptr);
		deallocMem((void *) ptr);
	}

	virtual ~Allocator() = default;
};

} // namespace stamon
