/*
	Name: ArrayList.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 28/07/23 17:10
	Description: 动态数组
*/

#pragma once

#include "IArrayList.hpp"
#include "vector"

namespace stamon::stdcpp {
template<typename T> class ArrayList {
	std::vector<T> list;

public:
	ArrayList()
		: list() {
	}

	ArrayList(int size)
		: list(size) {
	}

	void add(T value) {
		list.push_back(move(value));
	} // 末尾添加值

	void insert(int index, T value) {
		list.emplace(list.begin() + index, move(value));
	} // 将value插入到[index]

	void erase(int index) {
		list.erase(list.begin() + index);
	} // 删除[index]

	T at(int index) const {
		return list.at(index);
	} // 获取[index]

	bool empty() const {
		return list.empty();
	} // 判断是否为空

	void clear() {
		list.clear();
	} // 清空列表

	int size() const {
		return list.size();
	} // 获得元素个数

	ArrayList<T> operator+(ArrayList<T> src) const {
		ArrayList<T> rst(size() + src.size());

		for (int i = 0, len = size(); i < len; i++) {
			rst.list[i] = list[i];
		}

		for (int i = 0, offset = size(), len = src.size(); i < len; i++) {
			rst.list[offset + i] = move(src.list[i]);
		}

		return rst;
	}

	ArrayList<T> &operator+=(ArrayList<T> src) {
		for (int i = 0, len = src.size(); i < len; i++) {
			add(move(src[i]));
		}
		return *this;
	}

	T &operator[](int index) {
		return list[index];
	}

	T operator[](int index) const {
		return list[index];
	}
};
} // namespace stamon::stdcpp

namespace stamon {

template<typename T>
using ArrayList = interface::IArrayList<T, stdcpp::ArrayList<T>>;

} // namespace stamon