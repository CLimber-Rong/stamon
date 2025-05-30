/*
	Name: Stack.cpp
	License: Apache 2.0
	Author: 瞿相荣 
	Date: 17/01/23 18:06
	Description: 栈库，基于stack.h
*/
#pragma once

template <typename T>
class Stack {
	public:
		Stack();								//构造函数 
		int clear();							//清空
		int destroy();							//销毁
		int empty();							//是否为空
		int size();								//元素个数
		T* peek();							//获取栈顶元素
		int push(T* data); 					//入栈
		T* pop();							//出栈
		int traverse(STACK_VISIT visit); 		//从栈底到栈顶依次遍历，具体操作见stack.h 
};