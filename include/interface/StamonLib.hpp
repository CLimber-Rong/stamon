/*
	Name: StamonLib.hpp
	License: Apache 2.0
	Author: CLimber-Rong, GusemFowage
	Date: 12/08/23 23:24
	Description: 该库用于存储不涉及任何平台操作的轻量功能实现
*/

#pragma once

// 防止非标准环境中，NULL未定义
#ifndef NULL
#define NULL 0
#endif

// move和forward的实现
namespace stamon {

/*
 * remove_reference用于移除类型的引用
 * 传入T或T&或T&&，都返回T
 */

template<class T> struct remove_reference {
	typedef T type;
};
template<class T> struct remove_reference<T &> {
	typedef T type;
};
template<class T> struct remove_reference<T &&> {
	typedef T type;
};

/*
 * forward用于将左值或右值不加修饰的传递给其他代码，即完美转发
 * 使用forward可以减少不必要的拷贝复制
 */

template<typename T>
constexpr T &&forward(typename remove_reference<T>::type &t) {
	return (T &&) (t);
}

template<typename T>
constexpr T &&forward(typename remove_reference<T>::type &&t) {
	return (T &&) (t);
}

/*
 * move用于将给定的参数强转成右值，即移动语义
 * 如果某个被传递左值在后续不被需要，可以使用move转为左值，减少不必要的拷贝次数
 */

template<typename T>
constexpr typename remove_reference<T>::type &&move(T &&t) {
	return (typename remove_reference<T>::type &&) (t);
}

} // namespace stamon

// 用于简化宏

#define MACRO_START do {
#define MACRO_END \
	} \
	while (0)
