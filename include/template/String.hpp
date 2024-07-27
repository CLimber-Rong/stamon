/*
	Name: String.hpp
	Copyright: Apache2.0
	Author: CLimber-Rong, GusemFowage
	Date: 29/07/23 12:59
	Description: 字符串库
*/

/*
 * 一些网络上的大牛，他们告诉我，我必须在一些函数的结尾加上const限定符，才能支持
 const String var_name;
 * 的用法。
 * 我采纳了他们的建议，感谢他们！
 * ——CLimber-Rong
*/

#pragma once

#include"stmlib.hpp"

class String {
public:
	String();
	String(const char_type* str);
    String(const char_type* str, size_type sz);
	String(const String& str);
	String(String&& str);
	~String();

	String& operator=(const String& str);
	explicit operator const char_type*() const;	//要求显示转换
	size_type length() const;	//获取长度
	bool empty() const;	//判断是否为空

	String& append(const char_type* str);	//追加
	String& append(const String& str);
    void push_back(char_type c);	//追加一个字符
    void pop_back();	//弹出一个字符

	bool equals(const String& str) const;		//判等
	bool equals(const char_type* str) const;
	char_type& at(size_type i);		//获取下标
	char_type at(size_type i) const;
	const char_type* getstr() const;

	String substring(size_type pos, size_type len) const;
public:	// 运算符重载
	char_type& operator[](size_type i);
	char_type operator[](size_type i) const;
	String& operator+=(const String& str);
	String& operator+=(const char_type* str);
	String& operator+=(char_type c);
	bool operator==(const String& str) const;
	bool operator==(const char_type* str) const;
	bool operator!=(const String& str) const;
	bool operator!=(const char_type* str) const;

	bool operator<(const String& str) const;
	bool operator>(const String& str) const;
	bool operator<=(const String& str) const;
	bool operator>=(const String& str) const;
public:
	template<class s_t>
	friend String operator+(const s_t& str1, const s_t& str2);

	template<class s_t>
	String& operator+=(const s_t& str);

	int toInt() const;
	int toIntX() const;
	float toFloat() const;
	double toDouble() const;

	String(int v);
	String(float v);
	String(double v);
};

template<class T>
String toString(T&& t){
	return String(t);
}