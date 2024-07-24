/*
	Name: String.hpp
	Copyright: Apache2.0
	Author: CLimber-Rong	// 原作者
	Date: 29/07/23 12:59
	Description: 字符串库
	@brief: 字符串类
*/
#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../usetool/byte_get.hpp"

#include <string>

/**
 * @author: Gusem Fowage
 * @action: 更新字符串类（待添加...）
 * @notice: 如果未被添加，请联系
 */

class String {
	typedef char  	char_type;
	typedef len_t 	size_type;
private:
	char_type* data;
	size_type size;
	enum { local_capacity = 15 / sizeof(char_type) };

	union {
		// 短字符串优化
		char_type        cache[local_capacity+1]{0};
		// 已分配长度
		size_type        allocated_capacity;
	};

	// 重新分配内存 返回：新的内存地址
	char_type* reSalloc(size_type capacity) {
		capacity*= sizeof(char_type);
		if (capacity>=local_capacity) {
			char_type* addr=(char_type*)calloc(capacity+1, 1);
            strcpy(addr, data);	// 复制原数据
            allocated_capacity = capacity;	// 更新已分配长度
			return addr;
		}
		else return cache;	// 短字符串优化
	}
public:
	String() noexcept : data(cache), size(0) {}
	String(const char_type* str) noexcept : String() {
		size = strlen(str);
		data = reSalloc(size);
		strcpy(data, str);
	}
    String(const char_type* str, size_type sz) noexcept : String() {
		size = sz;
		data = reSalloc(sz);
		strncpy(data, str, sz);
	}
	String(const String& str) noexcept : String() {
		data = reSalloc(str.size);
		strcpy(data, str.data);
		size = str.size;
	}
	String(String&& str) noexcept {
		if(str.data == str.cache) {
			data = cache;
			strcpy(data, str.data);
			size = str.size;
		} else {
			data = str.data, size = str.size;
			allocated_capacity = str.allocated_capacity;

			str.data = str.cache;
			str.size = 0;
		}
	}
	~String() {
		if (data != cache) free(data);
	}
	String& operator=(const String& str) {
		if (this == &str) return *this;
		reSalloc(str.size);
		strcpy(data, str.data);
		size = str.size;
		return *this;
	}
	explicit operator const char_type*() const {
		return data;
	}
	size_type length() const {
		return size;
	}
	bool empty() const {
		return size == 0;
	}
	
	String& append(const char_type* str) {
		size_type len = size+strlen(str);
        if (data == cache || len > allocated_capacity) {
			data=reSalloc(len);
			strcpy(data+size, str);
		} else {
			strcpy(data+size, str);
		}
        size = len;
		return *this;
	}
	String& append(const String& str) {
		return append(str.data);
	}
    void push_back(char_type c) {
        size_type len = size+1;
        if (data == cache || len > allocated_capacity) {
			data=reSalloc(len);
		}
        data[size] = c;
        size = len;
    }
    void pop_back() {
		// 分配长度不变化，直接修改最后一个字符为'\0'
        data[size-1] = '\0';
        size--;
    }

	bool equals(const String& str) const {
		if (size != str.size) return false;
		return strcmp(data, str.data) == 0;
	}
	bool equals(const char_type* str) const {
		return strcmp(data, str) == 0;
	}
	char_type& at(size_type i) {
		if (i >= size); // throw out_of_range("String::at");
		return data[i];
	}
	char_type at(size_type i) const {
		if (i >= size); // throw out_of_range("String::at");
		return data[i];
	}
	const char_type* getstr() const noexcept {
		return data;
	}

	String substring(size_type pos, size_type len) const {
		if (pos >= size) return String();
		if (pos+len > size) len = size-pos;
		String s(data+pos, len);
		return s;
	}
public:	// 运算符重载
#if 1
	char_type& operator[](size_type i) noexcept {
		return data[i];
	}
	char_type operator[](size_type i) const noexcept{
		return data[i];
	}
	String& operator+=(const String& str) {
		return append(str);
	}
	String& operator+=(const char_type* str) {
		return append(str);
	}
	String& operator+=(char_type c) {
		push_back(c);
		return *this;
	}
	bool operator==(const String& str) const {
		return equals(str);
	}
	bool operator==(const char_type* str) const {
		return equals(str);
	}
	bool operator!=(const String& str) const {
		return !equals(str);
	}
	bool operator!=(const char_type* str) const {
		return !equals(str);
	}
	
	bool operator<(const String& str) const {
		for (size_type i = 0; i < size && i < str.size; i++) { 
			if (data[i] < str.data[i]) return true;
		}
		return false;
	}
	bool operator>(const String& str) const {
		for (size_type i = 0; i < size && i < str.size; i++) { 
			if (data[i] > str.data[i]) return true;
		}
		return false;
	}
	bool operator<=(const String& str) const {
		return !(*this > str);
	}
	bool operator>=(const String& str) const {
		return !(*this < str);
	}
	// int operator<=>(const String& str) const;
#endif
public:
	template<class s_t>
	friend String operator+(const s_t& str1, const s_t& str2) {
		String s(str1);
		s.append(str2);
		return s;
	}

	int toInt() const {
		int rst;
		sscanf(data, "%d", &rst);
		return rst;
	}

	int toIntX() const {
		int rst;
		sscanf(data, "%x", &rst);
		return rst;
	}

	float toFloat() const {
		float rst;
		sscanf(data, "%f", &rst);
		return rst;
	}

	double toDouble() const {
		double rst;
		sscanf(data, "%lf", &rst);
		return rst;
	}
};

template<class T>
String toString(T&& t){
	// 你自己实现，我不帮忙了
	String s(std::to_string(t).c_str());
	return s;
}