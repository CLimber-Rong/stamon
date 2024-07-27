/*
	Name: String.hpp
	Copyright: Apache2.0
	Author: GusemFowage, CLimber-Rong
	Date: 29/07/23 12:59
	Description: 跨平台类型定义库
*/

#include"stdint.h"
#include"stddef.h"

#define INT8 int8_t
#define INT16 int16_t
#define INT32 int32_t
#define INT64 int64_t
#define UINT8 uint8_t
#define UINT16 uint16_t
#define UINT32 uint32_t
#define UINT64 uint64_t

// 可以用于去除 符号 限定
// 以及 无符号 限定
// 可以用于转换 类型至相应长度的类型(与 union 配合使用)

template<size_t N, bool sign>
struct bytes_get{
	typedef void type;
};
template<>
struct bytes_get<1, true>{typedef INT8 type;};
template<>
struct bytes_get<2, true>{typedef INT16 type;};
template<>
struct bytes_get<4, true>{typedef INT32 type;};
template<>
struct bytes_get<8, true>{typedef INT64 type;};
// struct bytes_get<16, true>{typedef int8_t type;};
template<>
struct bytes_get<1, false>{typedef UINT8 type;};
template<>
struct bytes_get<2, false>{typedef UINT16 type;};
template<>
struct bytes_get<4, false>{typedef UINT32 type;};
template<>
struct bytes_get<8, false>{typedef UINT64 type;};

using len_t = bytes_get<sizeof(size_t), false>::type;
//等价于#define len_t bytes_get<sizeof(size_t), false>::type