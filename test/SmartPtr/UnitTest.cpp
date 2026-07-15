/*
	Name: UnitTest.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 13/07/26 22:14
	Description: SmartPtr单元测试，由AI生成
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "SmartPtr.hpp"

using namespace stamon;

// 辅助结构体，用于测试构造与析构
struct TestObj {
	int value;
	TestObj(int v)
		: value(v) {
	}
};

// 辅助结构体，用于测试析构行为
struct DestructorTracker {
	static int destroyCount;
	int value;
	DestructorTracker(int v)
		: value(v) {
	}
	~DestructorTracker() {
		destroyCount++;
	}
};
int DestructorTracker::destroyCount = 0;

// 测试：基本构造与get()
TEST_CASE("Basic construction and get") {
	TestObj *raw = config::DefaultAllocator.alloc<TestObj>(42);
	{
		SmartPtr<TestObj> sp(raw);
		REQUIRE(sp.get() == raw);
		REQUIRE(sp.get()->value == 42);
	}
}

// 测试：operator->()
TEST_CASE("Arrow operator") {
	TestObj *raw = config::DefaultAllocator.alloc<TestObj>(100);
	SmartPtr<TestObj> sp(raw);
	REQUIRE(sp->value == 100);
	sp->value = 200;
	REQUIRE(sp->value == 200);
}

// 测试：operator*()
TEST_CASE("Dereference operator") {
	TestObj *raw = config::DefaultAllocator.alloc<TestObj>(7);
	SmartPtr<TestObj> sp(raw);
	REQUIRE((*sp).value == 7);
	(*sp).value = 14;
	REQUIRE((*sp).value == 14);
}

// 测试：拷贝语义（共享引用计数）
TEST_CASE("Copy semantics") {
	TestObj *raw = config::DefaultAllocator.alloc<TestObj>(99);
	SmartPtr<TestObj> sp1(raw);
	SmartPtr<TestObj> sp2 = sp1;
	// 两个SmartPtr应指向同一对象
	REQUIRE(sp1.get() == sp2.get());
	REQUIRE(sp1.get() == raw);
	REQUIRE(sp2->value == 99);
	// 通过一个指针修改，另一个应可见
	sp1->value = 50;
	REQUIRE(sp2->value == 50);
}

// 测试：引用计数归零时析构函数被调用
TEST_CASE("Destructor called on zero ref count") {
	DestructorTracker::destroyCount = 0;
	{
		SmartPtr<DestructorTracker> sp(
				config::DefaultAllocator.alloc<DestructorTracker>(1));
		REQUIRE(sp->value == 1);
		REQUIRE(DestructorTracker::destroyCount == 0);
	}
	// 离开作用域后，对象应被析构
	REQUIRE(DestructorTracker::destroyCount == 1);
}

// 测试：多个SmartPtr共享时，仅最后释放时析构
TEST_CASE("Shared destructor behavior") {
	DestructorTracker::destroyCount = 0;
	{
		SmartPtr<DestructorTracker> sp1(
				config::DefaultAllocator.alloc<DestructorTracker>(2));
		{
			SmartPtr<DestructorTracker> sp2 = sp1;
			REQUIRE(sp1.get() == sp2.get());
			REQUIRE(DestructorTracker::destroyCount == 0);
		}
		// sp2离开作用域，但sp1仍持有引用，不应析构
		REQUIRE(DestructorTracker::destroyCount == 0);
	}
	// sp1也离开作用域，引用计数归零，应析构
	REQUIRE(DestructorTracker::destroyCount == 1);
}

// 测试：赋值操作
TEST_CASE("Assignment operator") {
	TestObj *raw1 = config::DefaultAllocator.alloc<TestObj>(10);
	TestObj *raw2 = config::DefaultAllocator.alloc<TestObj>(20);
	SmartPtr<TestObj> sp1(raw1);
	SmartPtr<TestObj> sp2(raw2);
	REQUIRE(sp1->value == 10);
	REQUIRE(sp2->value == 20);
	sp1 = sp2;
	// 赋值后sp1应与sp2指向同一对象
	REQUIRE(sp1.get() == sp2.get());
	REQUIRE(sp1->value == 20);
}

// 测试：const场景下的访问
TEST_CASE("Const access") {
	TestObj *raw = config::DefaultAllocator.alloc<TestObj>(55);
	const SmartPtr<TestObj> sp(raw);
	REQUIRE(sp.get() == raw);
	REQUIRE(sp->value == 55);
	REQUIRE((*sp).value == 55);
}