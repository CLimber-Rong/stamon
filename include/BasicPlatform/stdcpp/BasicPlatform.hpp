/*
	Name: BasicPlatform.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 31/08/25 22:04
	Description: 基本的平台库
*/

#pragma once

#define NEWDELETE_DEFINED

#include "IBasicPlatform.hpp"
#include "StamonLib.hpp"
#include "cstdint"
#include "cstring"

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
