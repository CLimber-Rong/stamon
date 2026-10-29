/*
	Name: UnitTest.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 11/07/26 21:16
	Description: HashMap的单元测试，由AI生成
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "String.hpp"
// String置于上面
#include "HashMap.hpp"
#include "doctest.h"

using namespace stamon;

// 测试默认构造
TEST_CASE("Default constructor") {
	HashMap<String, int> map;
}

// 测试put和get
TEST_CASE("Put and get") {
	HashMap<String, int> map;
	map.put("one", 1);
	map.put("two", 2);
	map.put("three", 3);

	REQUIRE(map.get("one") == 1);
	REQUIRE(map.get("two") == 2);
	REQUIRE(map.get("three") == 3);
}

// 测试链式put调用
TEST_CASE("Chained put calls") {
	HashMap<String, int> map;
	map.put("a", 10).put("b", 20).put("c", 30);

	REQUIRE(map.get("a") == 10);
	REQUIRE(map.get("b") == 20);
	REQUIRE(map.get("c") == 30);
}

// 测试put覆盖已有键
TEST_CASE("Put overwrite existing key") {
	HashMap<String, int> map;
	map.put("key", 100);
	REQUIRE(map.get("key") == 100);

	// 再次put同一个键，值应被覆盖
	map.put("key", 999);
	REQUIRE(map.get("key") == 999);
}

// 测试exist
TEST_CASE("Exist") {
	HashMap<String, int> map;
	map.put("exist_key", 42);

	REQUIRE(map.exist("exist_key") == true);
	REQUIRE(map.exist("no_key") == false);
}

// 测试del
TEST_CASE("Del") {
	HashMap<String, int> map;
	map.put("to_delete", 1);
	map.put("to_keep", 2);

	REQUIRE(map.exist("to_delete") == true);
	map.del("to_delete");
	REQUIRE(map.exist("to_delete") == false);

	// 被删除的键不影响其他键
	REQUIRE(map.get("to_keep") == 2);
}

// 测试getKeyList
TEST_CASE("Get key list") {
	HashMap<String, int> map;
	map.put("x", 1);
	map.put("y", 2);
	map.put("z", 3);

	ArrayList<String> keys = map.getKeyList();
	REQUIRE(keys.size() == 3);

	// 验证所有键都存在于列表中（无序）
	bool foundX = false, foundY = false, foundZ = false;
	for (Size i = 0; i < keys.size(); i++) {
		if (keys[i] == "x")
			foundX = true;
		if (keys[i] == "y")
			foundY = true;
		if (keys[i] == "z")
			foundZ = true;
	}
	bool found = foundX && foundY && foundZ;
	REQUIRE(found);
}

// 测试getValList
TEST_CASE("Get value list") {
	HashMap<String, int> map;
	map.put("a", 10);
	map.put("b", 20);

	ArrayList<int> vals = map.getValList();
	REQUIRE(vals.size() == 2);

	// 验证所有值都存在于列表中（无序）
	bool found10 = false, found20 = false;
	for (Size i = 0; i < vals.size(); i++) {
		if (vals[i] == 10)
			found10 = true;
		if (vals[i] == 20)
			found20 = true;
	}

	bool found = found10 & found20;
	REQUIRE(found);
}

// 测试以int为键、String为值的HashMap
TEST_CASE("HashMap with int key and String value") {
	HashMap<int, String> map;
	map.put(1, String("one"));
	map.put(2, String("two"));

	REQUIRE(map.get(1) == "one");
	REQUIRE(map.get(2) == "two");
	REQUIRE(map.exist(1) == true);
	REQUIRE(map.exist(3) == false);
}

// 测试大量插入后的一致性
TEST_CASE("Bulk insert consistency") {
	HashMap<int, int> map;

	// 插入100个键值对
	for (int i = 0; i < 100; i++) {
		map.put(i, i * i);
	}

	// 验证所有键值对
	for (int i = 0; i < 100; i++) {
		REQUIRE(map.exist(i) == true);
		REQUIRE(map.get(i) == i * i);
	}

	// 删除一半
	for (int i = 0; i < 100; i += 2) {
		map.del(i);
	}

	// 验证删除后的状态
	for (int i = 0; i < 100; i++) {
		if (i % 2 == 0) {
			REQUIRE(map.exist(i) == false);
		} else {
			REQUIRE(map.exist(i) == true);
			REQUIRE(map.get(i) == i * i);
		}
	}
}