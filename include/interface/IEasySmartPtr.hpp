/*
	Name: IEasySmartPtr.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 03/10/25 12:15
	Description: 智能指针接口
*/

#pragma once

#include "BasicPlatform.hpp"
#include "StamonLib.hpp"

/*
 * 对于一些不会循环引用的数据结构，可以用智能指针来封装指针
 * 利用引用计数法，EasySmartPtr会记录有多少个数据正在使用该指针
 * 当没有任何数据使用该指针时，其可以被视作垃圾内存而删除
 */

namespace stamon {

template<typename T, class Impl> class IEasySmartPtr;

template<typename T, class Impl> class IEasySmartPtr : public Impl {
	//智能指针
public:
	IEasySmartPtr(T *pointer)
		: Impl(pointer) {
		// 直接传入指针，默认销毁方式为直接delete
	}
	IEasySmartPtr(T *pointer, void (*destroy_funcptr)(IEasySmartPtr *ptr))
		: Impl(pointer, destroy_funcptr) {
		// 传入指针，并指定销毁方式
	}
	IEasySmartPtr(Impl impl)
		: Impl(move(impl)) {
		// 复制构造函数
	}
	T *get() const {
		// 获取指针
		return Impl::get();
	}
	T *operator->() const {
		// 直接访问指针的成员
		return Impl::operator->();
	}
	T &operator[](int index) const {
		// 直接访问元素
		return Impl::operator[](index);
	}
	T &operator*() const {
		// 直接访问目标
		return Impl::operator*();
	}
};

} // namespace stamon