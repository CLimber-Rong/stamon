/*
	Name: IString.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 03/10/25 16:55
	Description: 字符串接口
*/

#pragma once

#include "StamonLib.hpp"

namespace stamon {

template<class Impl> class IString : public Impl {
	// 字符串，拷贝传递

public:
	IString()
		: Impl() {
		// 初始化为空字符串
	}
	IString(const typename Impl::char_type *s)
		: Impl(s) {
		// 初始化，将s复制到this
	}
	IString(const byte *s)
		: Impl(s) {
		// 将字节串解析为字符串
	}
	IString(const typename Impl::char_type *s, unsigned int len)
		: Impl(s, len) {
		// 将s解析为字符串，但只复制len个字符
	}
	IString(Impl s)
		: Impl(move(s)) {
	}

	bool equals(const typename Impl::char_type *s) const {
		// 判断this的内容是否与s相等，是则返回true，否则返回false
		return Impl::equals(s);
	}

	bool equals(IString s) const {
		return Impl::equals(move(s));
	}

	IString &append(const typename Impl::char_type *s) {
		// 将s追加到this末尾
		Impl::append(s);
        return *this;
	}

	IString &append(IString s) {
		Impl::append(move(s));
        return *this;
	}

	// 以下的一系列toString函数会将不同的数据类型转为IString后保存到this当中，返回this
	IString toString(int value) {
        return Impl::toString(value);
	}

	IString toString(bool value) {
		return Impl::toString(value);
	}

	IString toString(float value) {
		return Impl::toString(value);
	}

	IString toString(double value) {
		return Impl::toString(value);
	}

	IString toString(IString value) {
		return Impl::toString(move(value));
	}

	int toInt() const {
		return Impl::toInt();
	}

	int toIntX() const {
		return Impl::toIntX();
	}

	float toFloat() const {
		return Impl::toFloat();
	}

	double toDouble() const {
		return Impl::toDouble();
	}

	unsigned int length() const {
		// 返回字符串长度
		return Impl::length();
	}

	typename Impl::char_type at(unsigned int index) const {
		// 返回下标为index的字符
		return Impl::at(index);
	}

	typename Impl::char_type *getstr() const {
		// 如果你只需要一个只读用的typename Impl::char_type*字符串，getstr函数足矣
		return Impl::getstr();
	}

	IString substring(unsigned int start, unsigned int end) const {
		// 获取从start到end（不包含end）的子字符串
		return Impl::substring(start, end);
	}

	IString &operator=(const typename Impl::char_type *right) {
		Impl::operator=(right);
        return *this;
	}

	IString operator+(IString right) const {
		return Impl::operator+(move(right));
	}

	IString operator+(const typename Impl::char_type *right) const {
		return Impl::operator+(right);
	}

	IString &operator+=(IString right) {
		Impl::operator+=(move((Impl)right));
        return *this;
	}

	IString &operator+=(const typename Impl::char_type *right) {
		Impl::operator+=(right);
        return *this;
	}

	bool operator==(IString right) const {
		return Impl::operator==(move((Impl)right));
	}

	bool operator==(const typename Impl::char_type *right) const {
		return Impl::operator==(right);
	}

	bool operator!=(IString right) const {
		return Impl::operator!=(move((Impl)right));
	}

	bool operator!=(const typename Impl::char_type *right) const {
		return Impl::operator!=(right);
	}

	bool operator<(IString right) const {
		return Impl::operator<(move((Impl)right));
	}

	bool operator<(const typename Impl::char_type *right) const {
		return Impl::operator<(right);
	}

	bool operator>(IString right) const {
		return Impl::operator>(move((Impl)right));
	}

	bool operator>(const typename Impl::char_type *right) const {
		return Impl::operator>(right);
	}

	bool operator<=(IString right) const {
		return Impl::operator<=(move((Impl)right));
	}

	bool operator<=(const typename Impl::char_type *right) const {
		return Impl::operator<=(right);
	}

	bool operator>=(IString right) const {
		return Impl::operator>=(move((Impl)right));
	}

	bool operator>=(const typename Impl::char_type *right) const {
		return Impl::operator>=(right);
	}

	typename Impl::char_type &operator[](unsigned int index) {
		return Impl::operator[](index);
	}

	typename Impl::char_type operator[](unsigned int index) const {
		return Impl::operator[](index);
	}

	bool empty() const {
		return Impl::empty();
	}
};

template<class T, class Impl> IString<Impl> toString(const T &t) {
	return IString<Impl>().toString(t);
}

template<class Impl> unsigned int toHash(IString<Impl> s);

} // namespace stamon