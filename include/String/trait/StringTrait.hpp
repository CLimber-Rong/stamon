/*
	Name: StringTrait.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 28/06/26 00:25
	Description: Strring的接口约束限制
*/

#pragma once

#include "Allocator.hpp"
#include "Result.hpp"
#include "StamonLib.hpp"

namespace stamon::trait {
template<typename C>
concept StringTrait = requires(C obj, const C &obj_const, Allocator *allocator,
		const char *cstr, const Byte *bstr, Size len, Size index, Size start,
		Size end, unsigned int val_uint, int val_int, bool val_bool,
		float val_float, double val_double) {
	// 构造函数
	C(allocator); // 初始化为空字符串
	C();
	C(cstr, allocator); // 将char*字符串复制到this
	C(cstr);
	C(cstr, len, allocator); // 将char*字符串复制len个字符到this
	C(cstr, len);
	C(bstr, allocator); // 将字节串解析为字符串
	C(bstr);

	//具有字符类型
	typename C::char_type;

	requires Copyable<C>;
	requires Movable<C>;

	// 以下的一系列toString函数会将不同的数据类型转为String后保存到this当中，返回*this
	{ obj.toString(val_uint) }->SameAs<C &>;
	{ obj.toString(val_int) }->SameAs<C &>;
	{ obj.toString(val_bool) }->SameAs<C &>;
	{ obj.toString(val_float) }->SameAs<C &>;
	{ obj.toString(val_double) }->SameAs<C &>;

	// 普通的方法
	{ obj_const.length() }->SameAs<Size>; // 获取长度
	{ obj_const.at(index) }->SameAs<typename C::char_type>; // 获取第index个字符
	// 获取一个只读的char_type字符串
	{ obj_const.getstr() }->SameAs<const typename C::char_type *>;
	// 获取从start到end（不包含end）的子字符串
	{ obj_const.substring(start, end) }->SameAs<C>;
	{ obj_const.empty() }->SameAs<bool>; // 判断是否为空
	{ obj_const.toHash() }->SameAs<unsigned int>; // 获取哈希值

	// 运算符重载
	{ obj_const + cstr }->SameAs<C>;
	{ obj_const + obj_const }->SameAs<C>;
	{ obj += cstr }->SameAs<C &>;
	{ obj += obj_const }->SameAs<C &>;
	{ obj == cstr }->SameAs<bool>;
	{ obj == obj_const }->SameAs<bool>;
	{ obj != cstr }->SameAs<bool>;
	{ obj != obj_const }->SameAs<bool>;
	{ obj < cstr }->SameAs<bool>;
	{ obj < obj_const }->SameAs<bool>;
	{ obj > cstr }->SameAs<bool>;
	{ obj > obj_const }->SameAs<bool>;
	{ obj <= cstr }->SameAs<bool>;
	{ obj <= obj_const }->SameAs<bool>;
	{ obj >= cstr }->SameAs<bool>;
	{ obj >= obj_const }->SameAs<bool>;
	{ obj[index] }->SameAs<typename C::char_type &>;
	{ obj_const[index] }->SameAs<typename C::char_type>;
};

template<typename C> using checkStringTrait = checkTrait<C, StringTrait<C>>;

} // namespace stamon::trait