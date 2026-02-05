/*
	Name: IHashMap.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 03/10/25 16:24
	Description: 哈希表类接口
*/

#pragma once

#include "StamonLib.hpp"

namespace stamon::interface {

template<typename KeyType, typename ValType, class Impl>
class IHashMap : public Impl {
	// 使用引用传递
public:
	IHashMap()
		: Impl() {
	}
	void put(const KeyType& key, ValType val) {
		// 设置键值对
		return Impl::put(key, move(val));
	}
	ValType get(const KeyType& key) {
		// 根据键获取值
		return Impl::get(key);
	}
	void del(const KeyType& key) {
		// 根据键删除键值对
		return Impl::del(key);
	}
	bool exist(const KeyType& key) {
		// 根据键判断是否存在对应的键值对
		return Impl::exist(key);
	}
	ArrayList<KeyType> getKeyList() {
		// 获取一个由所有键组成的列表
		return Impl::getKeyList();
	}
	ArrayList<ValType> getValList() {
		// 获取一个由所有值组成的列表
		return Impl::getValList();
	}
};

} // namespace stamon