/*
	Name: String.hpp
	Copyright: Apache2.0
	Author: Gusem Fowage, CLimber-Rong
	Date: 29/07/23 12:59
	Description: 跨平台数据类型定义库
*/
#include <stdint.h>
#include <stddef.h>

// 可以用于去除 符号 限定
// 以及 无符号 限定
// 可以用于转换 类型至相应长度的类型(与 union 配合使用)

template<size_t N, bool sign>
struct bytes_get{
	typedef void type;
};
template<>
struct bytes_get<1, true>{typedef int8_t type;};
template<>
struct bytes_get<2, true>{typedef int16_t type;};
template<>
struct bytes_get<4, true>{typedef int32_t type;};
template<>
struct bytes_get<8, true>{typedef int64_t type;};
// struct bytes_get<16, true>{typedef int8_t type;};
template<>
struct bytes_get<1, false>{typedef uint8_t type;};
template<>
struct bytes_get<2, false>{typedef uint16_t type;};
template<>
struct bytes_get<4, false>{typedef uint32_t type;};
template<>
struct bytes_get<8, false>{typedef uint64_t type;};
// struct bytes_get<16, false>{typedef uint128_t type;};

using len_t = bytes_get<sizeof(size_t), false>::type;

// 一下功能仍然在测试阶段
// 实际加入到程序中仍需要时间
using nullptr_t = decltype(nullptr);

template<class T>
struct pointer {
	typedef T* 
		ptr_t;
	typedef T& 
		ref_t;
	typedef const T& 
		const_reference;
	typedef T 
		value_type;
	typedef bytes_get<sizeof(void*), false>::type 
		dif_t;

	pointer() noexcept
		: ptr(nullptr) {}
	pointer(nullptr_t) noexcept
		: ptr(nullptr) {}
	pointer(ptr_t p) noexcept
		: ptr(p) {}
	const_reference operator*() const noexcept
		{ return *ptr; }
	ref_t operator*() noexcept
		{ return *ptr; }
	pointer operator->() const noexcept
		{ return ptr; }
	dif_t operator-(const pointer& other) const noexcept 
		{ return ptr - other.ptr; }
	ref_t operator[](dif_t i) noexcept
		{ return ptr[i]; }
	const_reference operator[](dif_t i) const noexcept
		{ return ptr[i]; }
	operator bool() const noexcept
		{ return ptr!= nullptr; }
	operator ptr_t() const noexcept
		{ return ptr; }
	ptr_t get() const noexcept
		{ return ptr; }
private:
	ptr_t ptr;
};