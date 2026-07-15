/*
	Name: String.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 28/06/26 00:25
	Description: 字符串的标准C++实现
*/

#pragma once

#include "Allocator.hpp"
#include "AllocatorAdapter.hpp"
#include "GlobalConfig.hpp"
#include "Result.hpp"
#include "StamonLib.hpp"
#include "StringTrait.hpp"
#include <charconv>
#include <string>

namespace stamon::stdcpp {

// 字符串相关的哈希函数定义
// 此哈希算法来源于Murmur Hash2，这里采用了基于MIT LICENSE发布的代码

unsigned int inline MurmurHash(const void *key, Size len, uint32_t seed) {
	const unsigned int m = 0x5bd1e995;
	const int r = 24;
	// 常量m和r是离线生成的混合阐述
	// 两者其实不是魔数，只是实践表明使用这两个数的效果良好

	unsigned int h = seed ^ len;
	// 将哈希值初始化为一个伪随机值

	const Byte *data = (const Byte *) key;
	// 将每四字节的数据混合到哈希值中

	while (len >= 4) {
		unsigned int k = *(unsigned int *) data;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	// 处理最后剩余的字节

	switch (len) {
	case 3:
		h ^= data[2] << 16;
	case 2:
		h ^= data[1] << 8;
	case 1:
		h ^= data[0];
		h *= m;
	};

	// 最终混合一遍哈希值，确保最后几个字节被充分混合

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

class String {
	using str_type = std::basic_string<char, std::char_traits<char>,
			AllocatorAdapter<char>>;

	str_type str;

	String(str_type str)
		: str(str) {
	}

public:
	using char_type = char;
	String(Allocator *allocator = &config::DefaultAllocator)
		: str(AllocatorAdapter<char>(allocator)) {
	}

	String(const char *cstr, Allocator *allocator = &config::DefaultAllocator)
		: str(cstr, AllocatorAdapter<char>(allocator)) {
	}

	String(const char *cstr, Size len,
			Allocator *allocator = &config::DefaultAllocator)
		: str(cstr, len, AllocatorAdapter<char>(allocator)) {
	}

	String(const Byte *bstr, Allocator *allocator = &config::DefaultAllocator)
		: String((const char *) bstr, allocator) {
	}

	String(const String &) = default;
	String(String &&) = default;
	String &operator=(const String &) = default;
	String &operator=(String &&) = default;

	String &toString(unsigned int val) {
		str = std::to_string(val);
		return *this;
	}
	String &toString(int val) {
		str = std::to_string(val);
		return *this;
	}
	String &toString(bool val) {
		str = std::to_string(val);
		return *this;
	}
	String &toString(float val) {
		str = std::to_string(val);
		return *this;
	}
	String &toString(double val) {
		str = std::to_string(val);
		return *this;
	}

	unsigned int toHash() const {
		return MurmurHash(
				getstr(), sizeof(String::char_type) * length(), 0x53544D);
		// 0x53544D是'STM'的ASCII码字节串，这里用作种子
	}

	Size length() const {
		return str.length();
	}

	char_type at(Size index) const {
		return str[index];
	}

	const char_type *getstr() const {
		return str.c_str();
	}

	String substring(Size start, Size end) const {
		return String(str_type(
				str.begin() + start, str.begin() + end, str.get_allocator()));
	}

	bool empty() const {
		return str.empty();
	}

	String operator+(String right) const {
		return str + stamon::move(right.str);
	}

	String operator+(const char *right) const {
		return str + right;
	}

	String &operator+=(const String &right) {
		str += right.str;
		return *this;
	}

	String &operator+=(const char *right) {
		str += right;
		return *this;
	}

	bool operator==(const String &right) const {
		return str == right.str;
	}

	bool operator==(const char *right) const {
		return str == right;
	}

	bool operator!=(const String &right) const {
		return str != right.str;
	}

	bool operator!=(const char *right) const {
		return str != right;
	}

	bool operator<(const String &right) const {
		return str < right.str;
	}

	bool operator<(const char *right) const {
		return str < right;
	}

	bool operator>(const String &right) const {
		return str > right.str;
	}

	bool operator>(const char *right) const {
		return str > right;
	}

	bool operator<=(const String &right) const {
		return str <= right.str;
	}

	bool operator<=(const char *right) const {
		return str <= right;
	}

	bool operator>=(const String &right) const {
		return str >= right.str;
	}

	bool operator>=(const char *right) const {
		return str >= right;
	}

	char_type &operator[](Size index) {
		return str[index];
	}

	char_type operator[](Size index) const {
		return str[index];
	}
};

static_assert(trait::StringTrait<String>);
} // namespace stamon::stdcpp

namespace stamon {

using String = trait::checkStringTrait<stdcpp::String>;

unsigned int toHash(const String &x) {
	return x.toHash();
}

} // namespace stamon