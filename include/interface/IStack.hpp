/*
	Name: IStack.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 03/10/25 16:45
	Description: 栈接口
*/

#pragma once

namespace stamon {

template<typename T, class Impl> class IStack : public Impl {
	// 栈，拷贝传递
public:
	void clear() {
		// 清空
		Impl::clear();
	}
	bool empty() {
		// 是否为空
		return Impl::empty();
	}
	int size() {
		// 元素个数
		return Impl::size();
	}
	T *peek() {
		// 获取栈顶元素
		return Impl::peek();
	}
	void push(T *data) {
		// 入栈
		Impl::push(data);
	}
	T *pop() {
		// 出栈
		return Impl::pop();
	}
};

} // namespace stamon