/*
	Name: BasicIo.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 19/09/25 12:36
	Description: 维持项目基本输入输出的库
*/

#pragma once

#include "IBasicIo.hpp"
#include "ctype.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

namespace stamon {

void platform_exit(int code) {
	exit(code);
}

int platform_system(String cmd) {
	return system(cmd.getstr());
}

#define _PLATFORM_INPUT_BUFFER_SIZE 1024

String platform_input() {
	String rst;

	char buffer[_PLATFORM_INPUT_BUFFER_SIZE]; // 缓冲区
	int pos = 0; // 当前缓冲区所处位置

	memset(buffer, 0, sizeof(buffer)); // 清空buffer

	char ch = getchar();

	// 先过滤先前的空格
	while (isspace(ch)) {
		ch = getchar();
	}

	while (!isspace(ch)) {
		if (pos == _PLATFORM_INPUT_BUFFER_SIZE - 2) {
			// 缓冲区已满
			rst += String(buffer);
			pos = 0;
			memset(buffer, 0, sizeof(buffer));
		}
		buffer[pos] = ch;
		pos++;
		ch = getchar();
	}

	if (pos != 0) {
		// 输入完成后，把缓冲区剩余的内容全部加入到rst当中
		rst += String(buffer);
	}

	return rst;
}

int platform_print(String s) {
	return printf("%s", s.getstr());
}

} // namespace stamon