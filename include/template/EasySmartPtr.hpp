/*
	Name: EasySmartPtr.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 30/03/25 12:52
	Description: 一个简单的智能指针
*/

#pragma once

template<typename T> class EasySmartPtr;

template<typename T> class EasySmartPtr {
public:
	EasySmartPtr(T *pointer); // 直接传入指针，默认销毁方式为直接delete

	EasySmartPtr(T *pointer, void (*destroy_funcptr)(EasySmartPtr<T> *ptr));
	// 传入指针，并指定销毁方式
	// 传入的销毁函数的类型必须是void(EasySmartPtr<T>*)

	EasySmartPtr(const EasySmartPtr &value); // 复制构造函数

	EasySmartPtr &operator=(const EasySmartPtr &value); // 赋值构造函数

	T *get(); // 获取指针

	T *operator->(); // 直接访问指针的成员

	~EasySmartPtr(); // 析构函数
};