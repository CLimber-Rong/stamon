/*
	Name: StamonLib.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 12/08/23 23:24
	Description: 该库用于存储不涉及任何平台操作的轻量功能实现
*/

#pragma once

namespace stamon {

/*
 * removeReference用于移除类型的引用
 * 传入T或T&或T&&，都返回T
 */

template<class T> struct removeReference {
	typedef T type;
};
template<class T> struct removeReference<T &> {
	typedef T type;
};
template<class T> struct removeReference<T &&> {
	typedef T type;
};

/*
 * forward用于将左值或右值不加修饰的传递给其他代码，即完美转发
 * 使用forward可以减少不必要的拷贝复制
 */

template<typename T>
constexpr T &&forward(typename removeReference<T>::type &t) {
	return (T &&) (t);
}

template<typename T>
constexpr T &&forward(typename removeReference<T>::type &&t) {
	return (T &&) (t);
}

/*
 * move用于将给定的参数强转成右值，即移动语义
 * 如果某个被传递左值在后续不被需要，可以使用move转为左值，减少不必要的拷贝次数
 */

template<typename T> constexpr typename removeReference<T>::type &&move(T &&t) {
	return (typename removeReference<T>::type &&) (t);
}

// 判断类型是否是数组，即T[]或T[N]

template<typename T> struct isArray {
	constexpr static bool value = false;
};

template<typename T> struct isArray<T[]> {
	constexpr static bool value = true;
};

template<typename T, unsigned int N> struct isArray<T[N]> {
	constexpr static bool value = true;
};

// 移除数组后缀

template<typename T> struct removeArray {
	typedef T type;
};

template<typename T> struct removeArray<T[]> {
	typedef T type;
};

template<typename T, unsigned int N> struct removeArray<T[N]> {
	typedef T type;
};

// 一个什么也不做的销毁函数，用于智能指针
template<typename T> void destroyNothing(T *p) {
	return;
}

// 交换函数
template<typename T> void swap(T &a, T &b) {
	T c = move(a);
	a = move(b);
	b = move(c);
}

using Size = decltype(sizeof(0));
using Byte = unsigned char;

// 在编译期判定类型是否一致
template<typename, typename> struct isSame {
	static constexpr bool value = false;
};

template<typename T> struct isSame<T, T> {
	static constexpr bool value = true;
};

template<typename T, typename U>
concept SameAs = isSame<T, U>::value && isSame<T, U>::value;

template<typename T> concept Copyable = requires(T lvalue, const T &rvalue) {
	T(rvalue);
	{ lvalue = rvalue }->SameAs<T &>;
};

template<typename T> concept Movable = requires(T lvalue, T &&rvalue) {
	T(rvalue);
	{ lvalue = rvalue }->SameAs<T &>;
};

template<typename T, bool Result> struct TraitChecker {
	static_assert(Result, "wrong trait");
	using type = T;
};

template<typename T, bool Result>
using checkTrait = typename TraitChecker<T, Result>::type;

struct Unit {};

// 哈希函数定义
unsigned int toHash(char x) {
	return (unsigned int) x;
}
unsigned int toHash(int x) {
	return (unsigned int) x;
}
unsigned int toHash(unsigned int x) {
	return x;
}
unsigned int toHash(float x) {
	auto rst = (unsigned int *) &x;
	return *rst;
}
unsigned int toHash(double x) {
	auto rst = (unsigned long long *) &x;
	return (unsigned int) (*rst) ^ (unsigned int) ((*rst) >> 32);
}
unsigned int toHash(Byte x) {
	return (unsigned int) x;
}

} // namespace stamon

// 用于简化宏

#define MACRO_START do {
#define MACRO_END \
	} \
	while (0)
