/*
	Name: SmartPtr.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 30/03/25 12:52
	Description: 一个简单的智能指针，使用了引用计数法
*/

#pragma once

#include "ISmartPtr.hpp"
#include "StamonLib.hpp"
#include "memory"

/*
 * 对于一些不会循环引用的数据结构，可以用智能指针来封装指针
 * 利用引用计数法，SmartPtr会记录有多少个数据正在使用该指针
 * 当没有任何数据使用该指针时，其可以被视作垃圾内存而删除
 */

namespace stamon::stdcpp {

template<typename _T> class SmartPtr {
	std::shared_ptr<_T> ptr;
	using T = typename stamon::remove_array<_T>::type;

public:
	SmartPtr(T *pointer)
		: ptr(pointer) {
	}
	// 直接传入指针，默认销毁方式为直接delete

	SmartPtr(T *pointer, void (*destroy_funcptr)(T *ptr))
		: ptr(pointer, destroy_funcptr) {
	}
	// 传入指针，并指定销毁方式

	T *get() const {
		return ptr.get();
	} // 获取指针

	T *operator->() const {
		return ptr.get();
	} // 直接访问指针的成员

	T &operator[](int index) const {
		return ptr[index];
	} // 直接访问元素

	T &operator*() const {
		return *ptr;
	}
};

} // namespace stamon::stdcpp

namespace stamon {

template<typename T>
using SmartPtr = interface::ISmartPtr<T, stdcpp::SmartPtr<T>>;

}