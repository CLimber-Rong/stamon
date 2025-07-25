/*
	Name: stmlib.hpp
	License: Apache 2.0
	Author: CLimber-Rong, GusemFowage
	Date: 12/08/23 23:24
	Description: 一些杂糅的库定义
*/

// 这个库原本叫做clib.h
// 由于需要引入c++代码，所以后缀改为hpp，顺便把文件名改为stmlib

#pragma once

#include "String.hpp"
#include "stdlib.h"
#include "ctype.h"

/*由于代码经常涉及到基类转派生类，所以我编写了这个可以直接转换的宏*/
/*
 * 这个宏的用法是：
 * dst = cast_class(转换后的类型,需要转换的值)
 */
#define cast_class(T, value) (*(T *) (&(value)))
#ifndef NULL
#define NULL 0
#endif

template<typename T, typename F> T cast_func(F f) {
	// 这个函数用于将类成员函数（也就是T）转为普通的函数（也就是F）
	union FT {
		T t;
		F f;
	};
	FT ft;
	ft.f = f;
	return ft.t; // 运用同一个地址，变相转换
}


template<class T>
String toString(const T& t){
	return String().toString(t);
}

#define MACRO_START do {

#define MACRO_END \
	} \
	while (0)

void platform_exit(int code) {
	exit(code);
}

int platform_system(String cmd) {
	return system(cmd.getstr());
}

#define _PLATFORM_INPUT_BUFFER_SIZE 1024

String platform_input() {

	String rst;

	char buffer[_PLATFORM_INPUT_BUFFER_SIZE];			//缓冲区
	int pos = 0;				//当前缓冲区所处位置

	memset(buffer, 0, sizeof(buffer));	//清空buffer

	char ch = getchar();

	//先过滤先前的空格
	while(isspace(ch)) {
		ch = getchar();
	}

	while(isspace(ch)==false) {
		if(pos == _PLATFORM_INPUT_BUFFER_SIZE-2) {
			//缓冲区已满
			rst += String(buffer);
			pos = 0;
			memset(buffer, 0, sizeof(buffer));
		}
		buffer[pos] = ch;
		pos++;
		ch = getchar();
	}

	if(pos!=0) {
		//输入完成后，把缓冲区剩余的内容全部加入到rst当中
		rst += String(buffer);
	}

	return rst;
}

int platform_print(String s) {
	return printf("%s",s.getstr());
}