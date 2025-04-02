/*
	Name: EasySmartPtr.hpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 30/03/25 12:52
	Description: 一个简单的智能指针，使用了引用计数法
*/

#pragma once

/*
 * 对于一些不会循环引用的数据结构，可以用智能指针来封装指针
 * 利用引用计数法，EasySmartPtr会记录有多少个数据正在使用该指针
 * 当没有任何数据使用该指针时，其可以被视作垃圾内存而删除
 */

template<typename T> class EasySmartPtr;

template<typename T> void __EASYSMARTPTR_DEFAULT_DESTROY_FUNCTION__(EasySmartPtr<T> *p);
// 默认的销毁函数

template<typename T> class EasySmartPtr {
public:
	void (*destroy_fp)(EasySmartPtr<T> *ptr);
	T *ptr;
	int *ref_cnt;

	EasySmartPtr(T *pointer) {
		// 初始化，传入指针
		ref_cnt = new int; // 初始化计数器
		(*ref_cnt) = 1; // 初始化时，只有一个SmartPointer拥有这个指针
		ptr = pointer;
		destroy_fp = __EASYSMARTPTR_DEFAULT_DESTROY_FUNCTION__<T>;
	} // 直接传入指针，默认销毁方式为直接delete

	EasySmartPtr(T *pointer, void (*destroy_funcptr)(EasySmartPtr<T> *ptr)) {
		ref_cnt = new int;
		(*ref_cnt) = 1;
		ptr = pointer;
		destroy_fp = destroy_funcptr;
	} // 传入指针，并指定销毁方式

	EasySmartPtr(const EasySmartPtr &value) {
		ref_cnt = value.ref_cnt; // 复制计数器
		ptr = value.ptr; // 复制指针
		destroy_fp = value.destroy_fp;
		(*ref_cnt)++; // 又多了一个EasySmartPtr指向这个指针
	} // 复制构造函数

	EasySmartPtr &operator=(const EasySmartPtr &value) {
		/*
		 * 将当前EasySmartPtr重新指定一个新的指针
		 * 就意味着需要抛弃当前的指针，指向新的指针
		 * 因此需要先减去当前指针的计数器，再将value的计数器加一
		 */

		if (&value == this) {
			// 自我赋值，直接返回即可
			return *this;
		}

		(*ref_cnt)--; // 减去当前计数器
		if ((*ref_cnt) == 0) {
			delete ref_cnt;
			destroy_fp(this);
			// 如果已经没有任何EasySmartPtr指向该指针
			// 那么此ptr可以被视作垃圾指针，需要被销毁
		}

		ref_cnt = value.ref_cnt; // 复制value
		ptr = value.ptr;
		destroy_fp = value.destroy_fp;

		(*ref_cnt)++; // value的计数器加一

		return *this;
	} // 赋值构造函数

	T *get() {
		return ptr;
	} // 获取指针

	T *operator->() {
		return ptr;
	} // 直接访问指针的成员

	~EasySmartPtr() {
		// 当前计数器需要减一
		(*ref_cnt)--;
		if ((*ref_cnt) == 0) {
			delete ref_cnt;
			destroy_fp(this);
			// 原理同上
		}
	} // 析构函数
};

template<typename T> void __EASYSMARTPTR_DEFAULT_DESTROY_FUNCTION__(EasySmartPtr<T> *p) {
	delete p->ptr;
}