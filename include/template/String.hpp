/*
	Name: String.hpp
	License: Apache2.0
	Author: CLimber-Rong, GusemFowage
	Date: 29/07/23 12:59
	Description: 字符串库
*/
#pragma once

typedef char_type;	//需自行定义char_type

class String {
	public:
		String();	   //初始化为空字符串

		String(char_type *s);
		String(char_type* s, unsigned int len);
		String(const String& s);
		String(String&& s);

		bool equals(const String& s) const;//判断this的内容是否与s相等，是则返回true，否则返回false

		bool equals(const char_type* s) const;

		explicit operator const char_type*() const;

		//以下的一系列toString函数会将不同的数据类型转为String后保存到this当中，返回*this
		String toString(int value);

		String toString(bool value);

		String toString(float value);

		String toString(double value);

		int toInt() const;

		int toIntX() const;

		float toFloat() const;

		double toDouble() const;

		unsigned int length() const;//返回字符串长度

		char_type at(unsigned int index) const;//返回第index个字符

		char_type* getstr() const;
		//如果你只需要一个只读用的char_type*字符串，getstr函数足矣

		String substring(unsigned int start, unsigned int end);

		String operator=(const String& right_value);

		String operator+(const String& right_value) const;
		
		template<class s_t>
		friend String operator+(const s_t& str1, const s_t& str2);

		String& operator+=(const String& s);

		template<class s_t>
		String& operator+=(const s_t& s);

		String& operator+=(const char_type* s);

		String& operator+=(char_type c);

		bool operator==(const String& right_value) const;

		bool operator==(const char_type* s) const;

		bool operator!=(const String& right_value) const;

		bool operator!=(const char_type* s) const;

		bool operator<(const String& s) const;

		bool operator>(const String& s) const;

		bool operator<=(const String& s) const;

		bool operator>=(const String& s) const;

		char_type& operator[](unsigned int index);

		char_type operator[](unsigned int index) const;

		bool empty() const;

		String& append(const char_type* s);

		String& append(const String& s);
		
		void push_back(char_type c);

		void pop_back();
		// 分配长度不变化，直接修改最后一个字符为'\0'

		~String();
};