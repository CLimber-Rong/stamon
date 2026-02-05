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
#include "stack"

namespace stamon::stdcpp {

template<typename T> class Stack {
	std::stack<T> stk;

public:
	void clear() {
		stk.clear();
	}
	bool empty() {
		return stk.empty();
	}
	int size() {
		return stk.size();
	}
	T peek() {
		return stk.top();
	}
	void push(T data) {
		stk.push(move(data));
	}
	T pop() {
		T rst = stk.top();
		stk.pop();
		return rst;
	}
};

} // namespace stamon::stdc

namespace stamon {

template<typename T>
using Stack = interface::IStack<T, stdcpp::Stack<T>>;

}