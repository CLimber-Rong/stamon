/*
	Name: Stack.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 17/01/23 18:06
	Description: 栈库
*/
#pragma once

#include "ArrayList.hpp"
#include "BasicPlatform.hpp"
#include "IStack.hpp"
#include "StamonLib.hpp"

namespace stamon::stdc {

template<typename T> class Stack {
	ArrayList<T *> list;

public:
	void clear() {
		list.clear();
	}
	bool empty() {
		return list.empty();
	}
	int size() {
		return list.size();
	}
	T *peek() {
		return list[list.size() - 1];
	}
	void push(T *data) {
		list.add(data);
	}
	T *pop() {
		T *rst = list[list.size() - 1];
		list.erase(list.size() - 1);
		return rst;
	}
};

} // namespace stamon::stdc

namespace stamon {

template<typename T>
using Stack = IStack<T, stdc::Stack<T>>;

}