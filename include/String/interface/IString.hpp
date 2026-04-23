/*
	Name: IString.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 03/10/25 16:55
	Description: 字符串接口
*/

#pragma once

#include "StamonLib.hpp"

namespace stamon::interface {

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

	// 以下的一系列toString函数会将不同的数据类型转为IString后保存到this当中，返回this
	IString &toString(unsigned int value) {
        Impl::toString(value);
		return *this;
	}

	IString &toString(int value) {
        Impl::toString(value);
		return *this;
	}

	IString &toString(bool value) {
		Impl::toString(value);
		return *this;
	}

	IString &toString(float value) {
		Impl::toString(value);
		return *this;
	}

	IString &toString(double value) {
		Impl::toString(value);
		return *this;
	}

	IString &toString(const IString& value) {
		Impl::toString(value);
		return *this;
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

	IString operator+(const IString& right) const {
		return Impl::operator+(right);
	}

	IString operator+(const typename Impl::char_type *right) const {
		return Impl::operator+(right);
	}

	IString &operator+=(const IString& right) {
		Impl::operator+=(right);
        return *this;
	}

	IString &operator+=(const typename Impl::char_type *right) {
		Impl::operator+=(right);
        return *this;
	}

	bool operator==(const IString& right) const {
		return Impl::operator==(right);
	}

	bool operator==(const typename Impl::char_type *right) const {
		return Impl::operator==(right);
	}

	bool operator!=(const IString& right) const {
		return Impl::operator!=(right);
	}

	bool operator!=(const typename Impl::char_type *right) const {
		return Impl::operator!=(right);
	}

	bool operator<(const IString& right) const {
		return Impl::operator<(right);
	}

	bool operator<(const typename Impl::char_type *right) const {
		return Impl::operator<(right);
	}

	bool operator>(const IString& right) const {
		return Impl::operator>(right);
	}

	bool operator>(const typename Impl::char_type *right) const {
		return Impl::operator>(right);
	}

	bool operator<=(const IString& right) const {
		return Impl::operator<=(right);
	}

	bool operator<=(const typename Impl::char_type *right) const {
		return Impl::operator<=(right);
	}

	bool operator>=(const IString& right) const {
		return Impl::operator>=(right);
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

template<class Impl> unsigned int toHash(const IString<Impl>& s);

} // namespace stamon