/*
	Name: BasicPlatform.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 31/08/25 22:04
	Description: 基本的平台库
*/

#pragma once

#include "IBasicPlatform.hpp"
#include "StamonLib.hpp"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// 重载new和delete

namespace stamon::stdc {
inline void *alloc(unsigned int size) noexcept {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		puts("STAMON: FATAL ERROR: FROM ALLOC: NO MORE MEMORY!\n");
		exit(-1);
	}
	return ptr;
}
} // namespace stamon::stdc

#ifndef NEWDELETE_DEFINED

// 如果第三方库本身已经引入了new和delete的重载，那么就不用重复实现

void *operator new(stamon::size_t size) {
	return stamon::stdc::alloc(size);
}

void *operator new[](stamon::size_t size) {
	return stamon::stdc::alloc(size);
}

void *operator new(stamon::size_t size, void *ptr) {
	return ptr;
}

void *operator new[](stamon::size_t size, void *ptr) {
	return ptr;
}

void operator delete(void *ptr) {
	free(ptr);
}

void operator delete[](void *ptr) {
	free(ptr);
}

#endif

namespace stamon {

// 字节类型定义
using byte = Ibyte<uint8_t>;

// 哈希函数定义
unsigned int toHash(char x) {
	return (unsigned int) x;
}
unsigned int toHash(int x) {
	return (unsigned int) x;
}
unsigned int toHash(unsigned int x) {
	return x;
}
unsigned int toHash(float x) {
	unsigned int rst;
	memcpy(&rst, &x, sizeof(unsigned int));
	return rst;
}
unsigned int toHash(double x) {
	unsigned long long rst;
	memcpy(&rst, &x, sizeof(unsigned long long));
	return (unsigned int) (rst) ^ (unsigned int) (rst >> 32);
}
unsigned int toHash(byte x) {
	return (unsigned int) x;
}

} // namespace stamon
