/*
	Name: IArrayList.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 20/08/25 13:18
	Description: 动态数组接口类
*/

#pragma once

#include "BasicPlatform.hpp"
#include "StamonLib.hpp"

namespace stamon::interface {

template<typename T, class Impl> class IArrayList : public Impl {
	// 列表，拷贝传递
public:
	IArrayList()
		: Impl() {
		// 构造空列表
	}
	IArrayList(int size)
		: Impl(size) {
		// 构造长度为size的列表
	}
	IArrayList(Impl impl)
		: Impl(move(impl)) {
	}
	void add(T value) {
		// 添加元素
		return Impl::add(move(value));
	}
	void insert(int index, T value) {
		// 插入元素
		return Impl::insert(index, move(value));
	}
	void erase(int index) {
		// 删除元素
		return Impl::erase(index);
	}
	T at(int index) const {
		// 获取元素
		return Impl::at(index);
	}
	bool empty() const {
		// 判断是否为空
		return Impl::empty();
	}
	void clear() {
		// 清空
		return Impl::clear();
	}
	int size() const {
		// 获取大小
		return Impl::size();
	}
	IArrayList operator+(IArrayList src) const {
		// 使用加法拼接
		return IArrayList(Impl::operator+(move(src)));
	}
	IArrayList &operator+=(IArrayList src) {
		// 拼接并赋值给自己
		Impl::operator+=(move(src));
		return *this;
	}
	T &operator[](int index) {
		// 获取元素
		return Impl::operator[](index);
	}
	T operator[](int index) const {
		// 获取元素
		return Impl::operator[](index);
	}
};

} // namespace stamon::interface