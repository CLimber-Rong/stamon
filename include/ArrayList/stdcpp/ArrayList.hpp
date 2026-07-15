/*
	Name: ArrayList.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 26/06/26 15:08
	Description: ArrayList的标准C++实现
*/

#pragma once

#include "Allocator.hpp"
#include "AllocatorAdapter.hpp"
#include "ArrayListTrait.hpp"
#include "GlobalConfig.hpp"
#include "StamonLib.hpp"
#include <vector>

namespace stamon::stdcpp {
template<typename T> class ArrayList {
	std::vector<T, AllocatorAdapter<T>> list;

public:
	ArrayList(Allocator *allocator = &config::DefaultAllocator)
		: list(AllocatorAdapter<T>(allocator)) {
	}

	ArrayList(int size, Allocator *allocator = &config::DefaultAllocator)
		: list(size, AllocatorAdapter<T>(allocator)) {
	}

	ArrayList &add(T value) {
		list.push_back(move(value));
		return *this;
	}

	ArrayList &insert(int index, T value) {
		list.emplace(list.begin() + index, move(value));
		return *this;
	}

	ArrayList &erase(int index) {
		list.erase(list.begin() + index);
		return *this;
	}

	T at(int index) const {
		return list.at(index);
	}

	bool empty() const {
		return list.empty();
	}

	ArrayList &clear() {
		list.clear();
		return *this;
	}

	Size size() const {
		return list.size();
	}

	ArrayList operator+(ArrayList src) const {
		ArrayList rst(size() + src.size());

		for (int i = 0, len = size(); i < len; i++) {
			rst.list[i] = list[i];
		}

		for (int i = 0, offset = size(), len = src.size(); i < len; i++) {
			rst.list[offset + i] = move(src.list[i]);
		}

		return rst;
	}

	ArrayList &operator+=(ArrayList src) {
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
using ArrayList = trait::checkArrayListTrait<stdcpp::ArrayList<T>, T>;
} // namespace stamon