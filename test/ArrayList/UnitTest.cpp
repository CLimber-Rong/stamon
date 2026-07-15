/*
	Name: UnitTest.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 11/07/26 21:06
	Description: ArrayList单元测试，由AI生成
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "ArrayList.hpp"
#include "String.hpp"
#include "doctest.h"

using namespace stamon;

// 测试默认构造和空列表
TEST_CASE("Default constructor and empty list") {
	ArrayList<int> list;
	REQUIRE(list.size() == 0);
	REQUIRE(list.empty() == true);
}

// 测试指定大小构造
TEST_CASE("Sized constructor") {
	ArrayList<int> list(5);
	REQUIRE(list.size() == 5);
	REQUIRE(list.empty() == false);
}

// 测试add和size
TEST_CASE("Add and size") {
	ArrayList<int> list;
	list.add(10);
	list.add(20);
	list.add(30);

	REQUIRE(list.size() == 3);
	REQUIRE(list.empty() == false);
	REQUIRE(list[0] == 10);
	REQUIRE(list[1] == 20);
	REQUIRE(list[2] == 30);
}

// 测试链式add调用
TEST_CASE("Chained add calls") {
	ArrayList<int> list;
	list.add(1).add(2).add(3);

	REQUIRE(list.size() == 3);
	REQUIRE(list[0] == 1);
	REQUIRE(list[1] == 2);
	REQUIRE(list[2] == 3);
}

// 测试insert
TEST_CASE("Insert") {
	ArrayList<int> list;
	list.add(1).add(3);
	list.insert(1, 2);
	// 期望: [1, 2, 3]

	REQUIRE(list.size() == 3);
	REQUIRE(list[0] == 1);
	REQUIRE(list[1] == 2);
	REQUIRE(list[2] == 3);

	// 在头部插入
	list.insert(0, 0);
	REQUIRE(list[0] == 0);
	REQUIRE(list.size() == 4);
}

// 测试erase
TEST_CASE("Erase") {
	ArrayList<int> list;
	list.add(10).add(20).add(30).add(40);

	list.erase(1);
	// 期望: [10, 30, 40]
	REQUIRE(list.size() == 3);
	REQUIRE(list[0] == 10);
	REQUIRE(list[1] == 30);
	REQUIRE(list[2] == 40);

	// 删除末尾元素
	list.erase(2);
	REQUIRE(list.size() == 2);
	REQUIRE(list[1] == 30);
}

// 测试at
TEST_CASE("At") {
	ArrayList<int> list;
	list.add(100).add(200).add(300);

	REQUIRE(list.at(0) == 100);
	REQUIRE(list.at(1) == 200);
	REQUIRE(list.at(2) == 300);
}

// 测试clear
TEST_CASE("Clear") {
	ArrayList<int> list;
	list.add(1).add(2).add(3);
	REQUIRE(list.size() == 3);

	list.clear();
	REQUIRE(list.size() == 0);
	REQUIRE(list.empty() == true);
}

// 测试operator+（列表合并）
TEST_CASE("Operator+ (list concatenation)") {
	ArrayList<int> a;
	a.add(1).add(2);

	ArrayList<int> b;
	b.add(3).add(4);

	ArrayList<int> c = a + b;
	// 期望: [1, 2, 3, 4]
	REQUIRE(c.size() == 4);
	REQUIRE(c[0] == 1);
	REQUIRE(c[1] == 2);
	REQUIRE(c[2] == 3);
	REQUIRE(c[3] == 4);

	// 原列表不应被修改
	REQUIRE(a.size() == 2);
	REQUIRE(b.size() == 2);
}

// 测试operator+=（列表追加）
TEST_CASE("Operator+= (list append)") {
	ArrayList<int> a;
	a.add(1).add(2);

	ArrayList<int> b;
	b.add(3).add(4);

	a += b;
	// 期望: a = [1, 2, 3, 4]
	REQUIRE(a.size() == 4);
	REQUIRE(a[0] == 1);
	REQUIRE(a[1] == 2);
	REQUIRE(a[2] == 3);
	REQUIRE(a[3] == 4);
}

// 测试operator[]的可写性
TEST_CASE("Writable operator[]") {
	ArrayList<int> list;
	list.add(10).add(20).add(30);

	list[1] = 99;
	REQUIRE(list[1] == 99);
	REQUIRE(list[0] == 10);
	REQUIRE(list[2] == 30);
}

// 测试存储String类型元素
TEST_CASE("Store String type elements") {
	ArrayList<String> list;
	list.add(String("hello"));
	list.add(String("world"));

	REQUIRE(list.size() == 2);
	REQUIRE(list[0] == "hello");
	REQUIRE(list[1] == "world");
}