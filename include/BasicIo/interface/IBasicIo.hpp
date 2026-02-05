/*
	Name: IBasicIo.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 03/10/25 11:45
	Description: 基本IO接口
*/

#pragma once

#include "String.hpp"

namespace stamon {

void platform_exit(int code);   //退出
int platform_system(String cmd);    //调用系统命令
String platform_input();    //输入
int platform_print(String s);   //输出

}