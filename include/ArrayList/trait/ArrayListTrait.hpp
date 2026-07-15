/*
	Name: ArrayListTrait.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 14/06/26 00:23
	Description: ArrayList的接口约束限制
*/

#pragma once

#include "Allocator.hpp"
#include "StamonLib.hpp"

namespace stamon::trait {
template<typename C, typename T>
concept ArrayListTrait = requires(C obj, const C &obj_const,
		Allocator *allocator, T value, const T value_const, Size size,
		Size index) {
	// C为ArrayList<T>，T为模板类型

	// 有关ArrayList的RAII
	C(allocator);
	C();
	C(size, allocator);
	C(size);
	requires Copyable<C>;
	requires Movable<C>;

	// 有关模板类型的RAII
	T();
	requires Copyable<T>;
	requires Movable<T>;

	// 普通的成员方法
	{ obj.add(value) }->SameAs<C &>; // 末尾追加value
	{ obj.insert(index, value) }->SameAs<C &>; // 在index处插入value
	{ obj.erase(index) }->SameAs<C &>; // 删除第index个元素
	{ obj_const.at(index) }->SameAs<T>; // 获取第index个元素
	{ obj_const.empty() }->SameAs<bool>; // 是否列表为空
	{ obj.clear() }->SameAs<C &>; // 清空列表
	{ obj_const.size() }->SameAs<Size>; // 获取列表长度
	{ obj_const + obj }->SameAs<C>; // 将两个列表拼接
	{ obj += obj }->SameAs<C &>; // 将两个列表拼接，并将拼接结果存入obj
	{ obj[index] }->SameAs<T &>; // 获取第index个元素
	{ obj_const[index] }->SameAs<T>; // 获取第index个元素
};

template<typename C, typename T>
using checkArrayListTrait = checkTrait<C, ArrayListTrait<C, T>>;

} // namespace stamon::trait