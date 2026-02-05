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
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

namespace stamon::stdc {

class String {
	/*
	 * 利用了缓存技术
	 * 对于短字符串（长度小于32字符），采用栈数组存储，减少内存申请次数
	 */
public:
	typedef char char_type;
	constexpr static int cache_size
			= 32; // 缓冲区长度，按sizeof(char_type)字节计

private:
	char_type *str = NULL;
	char_type cache[cache_size]{};

	void callocStr(int size) {
		// 直接分配内存，分配的内存置零
		if (size <= cache_size) {
			int i = 0;
			memset(cache, 0, sizeof(cache));
			str = cache;
		} else {
			str = (char_type *) calloc(size, sizeof(char_type));
		}
	}

	void reallocStr(int size) {
		// 重分配内存，扩展出的内存置零
		if (size <= cache_size) {
			// 栈转移到栈，什么都不用做
			if (str != cache) {
				// 堆转移到栈
				strcpy(cache, str);
				free(str);
				str = cache;
			}
		} else {
			if (str == cache) {
				// 栈转移到堆
				str = (char_type *) calloc(size, sizeof(char_type));
				strcpy(str, cache);
			} else {
				// 堆转移到堆
				int len = strlen(str); // 保留的原内容长度
				if (size < len) {
					len = size;
				}
				str = (char_type *) realloc(str, size * sizeof(char_type));
				for (int i = len; i < size; i++) {
					// 将扩展出的内存全部置零
					str[i] = 0;
				}
			}
		}
	}

	void freeStr() {
		if (str != cache) {
			free(str);
		}
	}

	void deepCopy(const String &s) {
		// 深拷贝
		callocStr(s.length() + 1);
		strcpy(str, s.getstr());
	}

	void shallowCopy(String &&s) {
		// 浅拷贝
		if (s.getstr() != s.cache) {
			// 如果目标的字符串创建在堆上，直接转移
			str = s.getstr();
		} else {
			deepCopy(s);
		}
		s.str = s.cache; // 指向栈缓存，这样就不会释放掉
	}

public:
	String() {
		callocStr(1);
	} // 初始化为空字符串

	String(const char_type *s) {
		callocStr(strlen(s) + 1);
		strcpy(str, s);
	} // 初始化，将s复制到this

	String(const byte *s) {
		callocStr(strlen((const char *) s) + 1);
		strcpy(str, (const char *) s);
	} // 将字节串解析为字符串

	String(const char_type *s, unsigned int len) {
		callocStr(len + 1);
		strncpy(str, s, len);
	} // 将s解析为字符串，但只复制len个字符

	String(const String &s) {
		deepCopy(s);
	}

	String(String &&s) {
		shallowCopy(move(s));
	}

	bool equals(const char_type *s) const {
		return strcmp(str, s) == 0;
	}

	bool equals(String s) const {
		return strcmp(str, s.getstr()) == 0;
	} // 判断this的内容是否与s相等，是则返回true，否则返回false

	String &append(const char_type *s) {
		unsigned int len = strlen(str) + strlen(s);

		reallocStr(len + 1);
		strcat(str, s);

		return *this;
	} // 将s追加到this末尾

	String &append(String s) {
		return append(s.getstr());
	}

	// 以下的一系列toString函数会将不同的数据类型转为String后保存到this当中，返回this
	String &toString(int value) {
		reallocStr(16);
		snprintf(str, 16, "%d", value);
		return *this;
	}

	String &toString(bool value) {
		reallocStr(8);

		if (value == true) {
			strcpy(str, "true");
		} else {
			strcpy(str, "false");
		}

		return *this;
	}

	String &toString(float value) {
		char_type s[256]; // 暂时存放生成的字符串
		memset(s, 0, sizeof(s)); // 置零
		snprintf(s, 256, "%f", value); // 打印到暂时的字符串数组
		reallocStr(strlen(s) + 1); // 根据生成的长度重分配内存
		strcpy(str, s);
		return *this;
	}

	String &toString(double value) {
		// 做法同上
		char_type s[256];
		memset(s, 0, sizeof(s));
		snprintf(s, 256, "%lf", value);
		reallocStr(strlen(s) + 1);
		strcpy(str, s);
		return *this;
	}

	String &toString(String value) {
		*this = move(value);
		return *this;
	}

	int toInt() const {
		int rst;
		sscanf(str, "%d", &rst);
		return rst;
	}

	int toIntX() const {
		int rst;
		sscanf(str, "%x", &rst);
		return rst;
	}

	float toFloat() const {
		float rst;
		sscanf(str, "%f", &rst);
		return rst;
	}

	double toDouble() const {
		double rst;
		sscanf(str, "%lf", &rst);
		return rst;
	}

	unsigned int length() const {
		return strlen(str);
	} // 返回字符串长度

	char_type at(unsigned int index) const {
		return str[index];
	} // 返回下标为index的字符

	char_type *getstr() const {
		return str;
	} // 如果你只需要一个只读用的char_type*字符串，getstr函数足矣

	String substring(unsigned int start, unsigned int end) const {
		// 获取从start到end（不包含end）的子字符串
		return String(str + start, end - start);
	}

	String &operator=(String right) {
		freeStr();
		shallowCopy(move(right));
		return *this;
	}

	String &operator=(const char_type *right) {
		freeStr();
		callocStr(strlen(right) + 1);
		strcpy(str, right);
		return *this;
	}

	String operator+(String right) const {
		return String(str).append(move(right));
	}

	String operator+(const char_type *right) const {
		return String(str).append(right);
	}

	String &operator+=(String right) {
		return append(move(right));
	}

	String &operator+=(const char_type *right) {
		return append(right);
	}

	bool operator==(String right) const {
		return equals(move(right));
	}

	bool operator==(const char_type *right) const {
		return equals(right);
	}

	bool operator!=(String right) const {
		return !equals(move(right));
	}

	bool operator!=(const char_type *right) const {
		return !equals(right);
	}

	bool operator<(String right) const {
		return strcmp(str, right.getstr()) < 0;
	}

	bool operator<(const char_type *right) const {
		return strcmp(str, right) < 0;
	}

	bool operator>(String right) const {
		return strcmp(str, right.getstr()) > 0;
	}

	bool operator>(const char_type *right) const {
		return strcmp(str, right) > 0;
	}

	bool operator<=(String right) const {
		return !(*this > move(right));
	}

	bool operator<=(const char_type *right) const {
		return !(*this > right);
	}

	bool operator>=(String right) const {
		return !(*this > move(right));
	}

	bool operator>=(const char_type *right) const {
		return !(*this > right);
	}

	char_type &operator[](unsigned int index) {
		return str[index];
	}

	char_type operator[](unsigned int index) const {
		return str[index];
	}

	bool empty() const {
		return length() == 0;
	}

	~String() {
		freeStr();
	}
};

} // namespace stamon::stdc

namespace stamon {

using String = interface::IString<stdc::String>;

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

unsigned int toHash(const String& s) {
	return MurmurHash(
			s.getstr(), sizeof(String::char_type) * s.length(), 0x53544D);
	// 0x53544D是'STM'的ASCII码字节串，这里用作种子
}

} // namespace stamon