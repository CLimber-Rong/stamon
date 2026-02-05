/*
	Name: String.hpp
	License: Apache2.0
	Author: CLimber-Rong, GusemFowage
	Date: 29/07/23 12:59
	Description: 字符串库
*/
#pragma once

#include "BasicPlatform.hpp"
#include "IString.hpp"
#include "cstdlib"
#include "string"

namespace stamon::stdcpp {

class String {
public:
	typedef char char_type;

private:
	std::string str;

public:
	String()
		: str() {
	} // 初始化为空字符串

	String(const char_type *s)
		: str(s) {
	} // 初始化，将s复制到this

	String(const byte *s)
		: str((const char *) s) {
	} // 将字节串解析为字符串

	String(const char_type *s, unsigned int len)
		: str((const char *) s, len) {
	} // 将s解析为字符串，但只复制len个字符

	String(std::string s)
		: str(stamon::move(s)) {
	}

	String(const String& s) : str(s.str) {
	}

	String(String&& s)
		: str(stamon::move(s.str)) {
	}

	String &operator=(const String& right) {
		str = right.str;
		return *this;
	}

	String &operator=(String&& right) {
		str = stamon::move(right.str);
		return *this;
	}

	String &operator=(const char_type *right) {
		str = std::string(right);
		return *this;
	}

	bool equals(const char_type *s) const {
		return str == std::string((const char *) s);
	}

	bool equals(String s) const {
		return str == s.str;
	} // 判断this的内容是否与s相等，是则返回true，否则返回false

	String &append(const char_type *s) {
		str += std::string((const char *) s);
		return *this;
	} // 将s追加到this末尾

	String &append(String s) {
		str += s.str;
		return *this;
	}

	// 以下的一系列toString函数会将不同的数据类型转为String后保存到this当中，返回this
	String &toString(int value) {
		str = std::to_string(value);
		return *this;
	}

	String &toString(bool value) {
		str = std::to_string(value);
		return *this;
	}

	String &toString(float value) {
		str = std::to_string(value);
		return *this;
	}

	String &toString(double value) {
		str = std::to_string(value);
		return *this;
	}

	String &toString(String value) {
		*this = stamon::move(value);
		return *this;
	}

	int toInt() const {
		return std::stoi(str);
	}

	int toIntX() const {
		int rst;
		sscanf(str.c_str(), "%x", &rst);
		return rst;
	}

	float toFloat() const {
		return std::stof(str);
	}

	double toDouble() const {
		return std::stod(str);
	}

	unsigned int length() const {
		return str.size();
	} // 返回字符串长度

	char_type at(unsigned int index) const {
		return str[index];
	} // 返回下标为index的字符

	char_type *getstr() const {
		return (char_type *) str.c_str();
	} // 如果你只需要一个只读用的char_type*字符串，getstr函数足矣

	String substring(unsigned int start, unsigned int end) const {
		// 获取从start到end（不包含end）的子字符串
		return String(str.substr(start, end-start));
	}

	String operator+(String right) const {
		return str + stamon::move(right.str);
	}

	String operator+(const char_type *right) const {
		return str + std::string(right);
	}

	String &operator+=(String right) {
		return append(stamon::move(right));
	}

	String &operator+=(const char_type *right) {
		return append(right);
	}

	bool operator==(String right) const {
		return equals(stamon::move(right));
	}

	bool operator==(const char_type *right) const {
		return equals(right);
	}

	bool operator!=(String right) const {
		return !equals(stamon::move(right));
	}

	bool operator!=(const char_type *right) const {
		return !equals(right);
	}

	bool operator<(String right) const {
		return str < right.str;
	}

	bool operator<(const char_type *right) const {
		return str < std::string(right);
	}

	bool operator>(String right) const {
		return str > right.str;
	}

	bool operator>(const char_type *right) const {
		return str > std::string(right);
	}

	bool operator<=(String right) const {
		return str <= right.str;
	}

	bool operator<=(const char_type *right) const {
		return str <= std::string(right);
	}

	bool operator>=(String right) const {
		return str >= right.str;
	}

	bool operator>=(const char_type *right) const {
		return str >= std::string(right);
	}

	char_type &operator[](unsigned int index) {
		return str[index];
	}

	char_type operator[](unsigned int index) const {
		return str[index];
	}

	bool empty() const {
		return str.empty();
	}
};

} // namespace stamon::stdcpp

namespace stamon {

using String = interface::IString<stdcpp::String>;

template<class T> String toString(const T &t) {
	return String().toString(t);
}

// 字符串相关的哈希函数定义
// 此哈希算法来源于Murmur Hash2，这里采用了基于MIT LICENSE发布的代码

uint32_t MurmurHash(const void *key, int len, uint32_t seed) {
	const uint32_t m = 0x5bd1e995;
	const int r = 24;
	// 常量m和r是离线生成的混合阐述
	// 两者其实不是魔数，只是实践表明使用这两个数的效果良好

	uint32_t h = seed ^ len;
	// 将哈希值初始化为一个伪随机值

	const byte *data = (const byte *) key;
	// 将每四字节的数据混合到哈希值中

	while (len >= 4) {
		uint32_t k = *(uint32_t *) data;

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

unsigned int toHash(const String &s) {
	return MurmurHash(
			s.getstr(), sizeof(String::char_type) * s.length(), 0x53544D);
	// 0x53544D是'STM'的ASCII码字节串，这里用作种子
}

} // namespace stamon