/*
	Name: String.hpp
	Copyright: Apache2.0
	Author: CLimber-Rong, GusemFowage
	Date: 29/07/23 12:59
	Description: 字符串库
*/
#pragma once

/*
 * 一些网络上的大牛，他们告诉我，我必须在一些函数的结尾加上const限定符，才能支持
 const String var_name;
 * 的用法。
 * 我采纳了他们的建议，感谢他们！
*/

#include"stdlib.h"
#include"string.h"
#include"stdio.h"

typedef char char_type;
typedef size_t size_type;

class String {

		/*
		 * 利用了缓存技术
		 * 对于短字符串（长度小于32字符），采用栈数组存储，减少内存申请次数
		 */

		char_type* str;
		char_type cache[32] = {0};

		void* StrCalloc(int size) {
			if(size*sizeof(char_type)<=32) {
				int i = 0;
				while((cache[i]!=0) && (i<32)) {
					cache[i] = 0;
					i++;
				}
				return cache;
			} else {
				return calloc(size, sizeof(char_type));
			}
		}

		void StrFree() {
			if(str!=cache) {
				free(str);
			}
		}

	public:
		String() {
			str = (char_type*)StrCalloc(1);
		}			   //初始化为空字符串

		String(char_type *s) {
			str = (char_type*)StrCalloc(strlen(s)+1);
			strcpy(str, s);
		}	   //初始化，将s复制到this

		String(char_type* s, size_type len) {
			str = (char_type*)StrCalloc(len+1);
			strncpy(str, s, len);
		}

		String(const String& s) {
			str = (char_type*)StrCalloc(s.length()+1);
			strcpy(str, s.getstr());
		}

		String(String&& s) {
			str = (char_type*)StrCalloc(s.length()+1);
			strcpy(str, s.getstr());
		}

		bool equals(const String& s) const {

			if(length()!=s.length()) {
				return false;
			}

			return strcmp(str, s.getstr())==0;

		} //判断this的内容是否与s相等，是则返回true，否则返回false

		bool equals(const char_type* s) const {
			return strcmp(str, str) == 0;
		}

		explicit operator const char_type*() const {
			return str;
		}

		//以下的一系列toString函数会将不同的数据类型转为String后保存到this当中，返回this
		String toString(int value) {
			StrFree();
			str = (char_type*)StrCalloc(256);
			sprintf(str, "%d", value);
			str = (char_type*)realloc(str, strlen(str)+1);
			return String(str);
		}

		String toString(bool value) {
			str = (char_type*)StrCalloc(10);

			if(value==true) {
				str = (char_type*)realloc(str, 5);
				strcpy(str, "true");
			} else {
				str = (char_type*)realloc(str, 6);
				strcpy(str, "false");
			}

			return String(str);
		}

		String toString(float value) {
			StrFree();
			str = (char_type*)StrCalloc(256);
			sprintf(str, "%f", value);
			str = (char_type*)realloc(str, strlen(str)+1);
			return String(str);
		}

		String toString(double value) {
			StrFree();
			str = (char_type*)StrCalloc(256);
			sprintf(str, "%lf", value);
			str = (char_type*)realloc(str, strlen(str)+1);
			return String(str);
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

		size_type length() const {
			return strlen(str);
		}			   //返回字符串长度

		char_type at(size_type index) const {
			return str[index];
		}		   //返回第index个字符

		char_type* getstr() const {
			return str;
		}	//如果你只需要一个只读用的char_type*字符串，getstr函数足矣

		String substring(size_type start, size_type end) {
			//获取从start到end（不包含end）的子字符串
			return String(str+start, end-start);
		}

		String operator=(const String& right_value) {
			StrFree();

			str = (char_type*)StrCalloc(right_value.length()+1);

			strcpy(str, right_value.getstr());

			return String(str);
		}

		String operator+(const String& right_value) const {
			return String(str).append(right_value);
		}
		
		template<class s_t>
		friend String operator+(const s_t& str1, const s_t& str2) {
			String s(str1);
			s.append(String(str2));
			return s;
		}

		String& operator+=(const String& s) {
			return append(s);
		}

		template<class s_t>
		String& operator+=(const s_t& s) {
			append(String(s));
			return *this;
		}

		String& operator+=(const char_type* s) {
			return append(s);
		}

		String& operator+=(char_type c) {
			push_back(c);
			return *this;
		}

		bool operator==(const String& right_value) const {
			return equals(right_value);
		}

		bool operator==(const char_type* s) const {
			return equals(s);
		}

		bool operator!=(const String& right_value) const {
			return !equals(right_value);
		}

		bool operator!=(const char_type* s) const {
			return !equals(s);
		}

		bool operator<(const String& s) const {
			return strcmp(str, s.getstr())>0;
		}

		bool operator>(const String& s) const {
			return strcmp(str, s.getstr())<0;
		}

		bool operator<=(const String& s) const {
			return !(*this > str);
		}

		bool operator>=(const String& s) const {
			return !(*this < str);
		}

		char_type& operator[](size_type index) {
			return str[index];
		}

		char_type operator[](size_type index) const {
			return str[index];
		}

		bool empty() const {
			return length()==0;
		}

		String& append(const char_type* s) {

			size_type len =  length()+strlen(s);

			if(str==cache && len<=32-1) {
				strcat(str, s);
			} else {
				char_type* temp = (char_type*)calloc(len+1, sizeof(char_type));
				strcpy(temp, str);
				strcat(temp, s);
				StrFree();
				str = temp;
			}

			return *this;
		}

		String& append(const String& s) {
			return append(s.getstr());
		}
		
		void push_back(char_type c) {
			append(String(&c));
			return;
		}

		void pop_back() {
			// 分配长度不变化，直接修改最后一个字符为'\0'
			str[length()-1] = '\0';
		}

		~String() {
			StrFree();
		}
};