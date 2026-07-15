/*
	Name: HashMapTrait.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 03/07/26 15:45
	Description: 哈希表的接口定义
*/

#pragma once

#include "Allocator.hpp"
#include "ArrayList.hpp"
#include "ArrayListTrait.hpp"
#include "StamonLib.hpp"

namespace stamon::trait {
template<typename C, typename Key, typename Val>
concept HashMapTrait = requires(
		C obj, const C &obj_const, Allocator *allocator, Key key, Val val) {
	C(allocator);
	C();
	requires Copyable<C>;
	requires Movable<C>;
	requires Movable<Key>;
	requires Movable<Val>;
	requires ArrayListTrait<ArrayList<Key>, Key>;
	requires ArrayListTrait<ArrayList<Val>, Val>;
	// 普通成员函数
	{ obj.put(key, val) }->SameAs<C &>; // 添加或修改键值对
	{ obj.del(key) }->SameAs<C &>; // 删除键值对
	{ obj_const.get(key) }->SameAs<Val>; // 获取键值对
	{ obj_const.exist(key) }->SameAs<bool>; // 判断键值对是否存在
	{ obj_const.getKeyList() }->SameAs<ArrayList<Key>>; // 获取所有键
	{ obj_const.getValList() }->SameAs<ArrayList<Val>>; // 获取所有值
};

template<typename C, typename Key, typename Val>
using checkHashMapTrait = checkTrait<C, HashMapTrait<C, Key, Val>>;

} // namespace stamon::trait