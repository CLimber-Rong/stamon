/*
	Name: StringMap.hpp
	License: Apache 2.0
	Author: 瞿相荣
	Date: 18/01/23 19:14
	Description: 字符串map
*/

#pragma once

#include "ByteMap.hpp"
#include "String.hpp"

template<typename T> class StringMap {
	ByteMap<T> map;

public:
	StringMap() {} // 构造方法
	int put(const String &s, T *data) {
		return map.put(s.getstr(), s.length(), data);
	} // 设置键值
	int del(const String &s, T *data) {
		return map.del(s.getstr(), s.length());
	} // 删除键值
	T *get(const String &s) {
		return map.get(s.getstr(), s.length());
	} // 获取值
	bool containsKey(const String &s) {
		return map.containsKey(s.getstr(), s.length());
	} // 是否存在该键
	int clear() {
		return map.clear();
	} // 清空
	bool empty() {
		return map.empty();
	} // 是否为空
	STRIE *getStrie() {
		return map.getStrie();
	}

	template<typename list_T> ArrayList<list_T> getValList() {
		return map.template getValList<list_T>();
	}
};