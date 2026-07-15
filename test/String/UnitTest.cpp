/*
	Name: UnitTest.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 11/07/26 20:46
	Description: 字符串单元测试，由AI生成
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "String.hpp"
#include <cstring>

using namespace stamon;

// 测试默认构造和空字符串
TEST_CASE("Default constructor and empty string") {
	String s;
	REQUIRE(s.length() == 0);
	REQUIRE(s.empty() == true);
}

// 测试从C字符串构造
TEST_CASE("C-string constructor") {
	String s("hello");
	REQUIRE(s.length() == 5);
	REQUIRE(s.empty() == false);
	REQUIRE(std::strcmp(s.getstr(), "hello") == 0);
}

// 测试从C字符串和指定长度构造
TEST_CASE("C-string with length constructor") {
	String s("hello world", 5);
	REQUIRE(s.length() == 5);
	REQUIRE(std::strcmp(s.getstr(), "hello") == 0);
}

// 测试拷贝构造
TEST_CASE("Copy constructor") {
	String s1("test");
	String s2(s1);
	REQUIRE(s2 == s1);
	REQUIRE(s2.length() == s1.length());
}

// 测试移动构造
TEST_CASE("Move constructor") {
	String s1("move me");
	String s2(std::move(s1));
	REQUIRE(s2 == "move me");
	REQUIRE(s2.length() == 7);
}

// 测试toString转换各类型
TEST_CASE("toString conversion") {
	String s;

	s.toString(42u);
	REQUIRE(s == "42");

	s.toString(-7);
	REQUIRE(s == "-7");

	s.toString(true);
	REQUIRE(s == "1");

	s.toString(false);
	REQUIRE(s == "0");
}

// 测试toHash的确定性
TEST_CASE("toHash determinism") {
	String s1("hash test");
	String s2("hash test");
	String s3("different");

	// 相同内容的哈希值应该相同
	REQUIRE(s1.toHash() == s2.toHash());
	// 不同内容的哈希值大概率不同
	REQUIRE(s1.toHash() != s3.toHash());
}

// 测试at和operator[]
TEST_CASE("at and operator[]") {
	String s("abcdef");
	REQUIRE(s.at(0) == 'a');
	REQUIRE(s.at(5) == 'f');
	REQUIRE(s[0] == 'a');
	REQUIRE(s[3] == 'd');

	// 测试可写下标
	s[0] = 'z';
	REQUIRE(s[0] == 'z');
}

// 测试substring
TEST_CASE("Substring") {
	String s("hello world");
	String sub = s.substring(0, 5);
	REQUIRE(sub == "hello");
	REQUIRE(sub.length() == 5);

	String sub2 = s.substring(6, 11);
	REQUIRE(sub2 == "world");
}

// 测试字符串拼接
TEST_CASE("String concatenation") {
	String s1("hello");
	String s2(" world");

	// operator+ 与String
	String s3 = s1 + s2;
	REQUIRE(s3 == "hello world");

	// operator+ 与C字符串
	String s4 = s1 + " stamon";
	REQUIRE(s4 == "hello stamon");

	// operator+=
	String s5("foo");
	s5 += "bar";
	REQUIRE(s5 == "foobar");

	String s6("abc");
	s6 += String("def");
	REQUIRE(s6 == "abcdef");
}

// 测试比较运算符
TEST_CASE("Comparison operators") {
	String a("apple");
	String b("banana");
	String c("apple");

	REQUIRE(a == c);
	REQUIRE(a != b);
	REQUIRE(a < b);
	REQUIRE(b > a);
	REQUIRE(a <= c);
	REQUIRE(a >= c);

	// 与C字符串比较
	REQUIRE(a == "apple");
	REQUIRE(a != "banana");
	REQUIRE(a < "banana");
}