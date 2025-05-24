/*
	Name: Stack.hpp
	Copyright: Apache 2.0
	Author: 瞿相荣
	Date: 17/01/23 18:06
	Description: 栈库，基于stack.h
*/
#pragma once

#include "ArrayList.hpp"

template<typename T> class Stack {
	ArrayList<T *> list;

public:
	Stack() {}
	int clear() {
		list.clear();
		return 1;
	} // 清空
	int empty() {
		return list.empty();
	} // 是否为空
	int size() {
		return list.size();
	} // 元素个数
	T *peek() {
		return list[list.size() - 1];
	} // 获取栈顶元素
	int push(T *data) {
		list.add(data);
		return 1;
	} // 入栈
	T *pop() {
		T *rst = list[list.size() - 1];
		list.erase(list.size() - 1);
		return rst;
	} // 出栈
};