/*
	Name: IBasicPlatform.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 03/10/25 11:48
	Description: 基本运行必需接口
*/

#pragma once

// 重载new和delete

#ifndef NEWDELETE_DEFINED
// 如果第三方库本身已经引入了new和delete的重载，那么就不用重复实现

namespace stamon {
using size_t = decltype(sizeof(0)); // 自定义size_t
}

void *operator new(stamon::size_t size);
void *operator new[](stamon::size_t size);
void *operator new(stamon::size_t size, void *ptr);
void *operator new[](stamon::size_t size, void *ptr);
void operator delete(void *ptr);
void operator delete[](void *ptr);
#define NEWDELETE_DEFINED
#endif

namespace stamon {

// 字节类型
template<typename T> using Ibyte = T;

// 哈希函数
unsigned int toHash(char x);
unsigned int toHash(int x);
unsigned int toHash(unsigned int x);
unsigned int toHash(float x);
unsigned int toHash(double x);

template<typename T> unsigned int toHash(Ibyte<T> x); // 字节数据类型的哈希

} // namespace stamon
