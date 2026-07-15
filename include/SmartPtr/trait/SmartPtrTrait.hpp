/*
	Name: SmartPtrTrait.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 12/07/26 15:25
	Description: 智能指针的接口约束
*/

#pragma once

#include "Allocator.hpp"
#include "StamonLib.hpp"

namespace stamon::trait {
template<typename C, typename T>
concept SmartPtrTrait = requires(const C &obj_const, Allocator *allocator,
		Allocator *deallocator, T *ptr, Size index) {
	// 构造函数
	C(ptr);
	C(ptr, allocator, deallocator);

	// RAII
	requires Movable<C>;
	requires Copyable<C>;

	// 普通的方法
	{ obj_const.get() }->SameAs<T *>; // 获取指针
	{ obj_const.operator->() }->SameAs<T *>; // 直接访问指针成员
	{ *obj_const }->SameAs<T &>; // 指针解引用
};

template<typename C, typename T>
using checkSmartPtrTrait = checkTrait<C, SmartPtrTrait<C, T>>;

} // namespace stamon::trait