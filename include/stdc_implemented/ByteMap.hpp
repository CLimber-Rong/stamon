/*
	Name: ByteMap.hpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 10/02/24 13:38
	Description: 字节串map
*/

#pragma once

#include "ArrayList.hpp"
#include "EasySmartPtr.hpp"
#include "Stack.hpp"
#include "strie.h"

void __SMARTSTRIE_DESTROY_FUNCTION__(EasySmartPtr<STRIE>* p) {
	DestroyTrie(p->ptr);
}

class SmartStrie : public EasySmartPtr<STRIE> {
public:
	SmartStrie()
		: EasySmartPtr<STRIE>(InitTrie(), __SMARTSTRIE_DESTROY_FUNCTION__) {
	}
};

template<typename T> class ByteMap {
	SmartStrie map;

public:
	ByteMap() {
	}

	int put(char *s, int size, T *data) { // 设置键值
		return SetTrieKeyVal(map.get(), (unsigned char *) s, size, (void *) data);
	}
	int del(char *s, int size) { // 删除键值
		return DelTrieKeyVal(map.get(), (unsigned char *) s, size);
	}
	T *get(char *s, int size) { // 获取值
		return (T *) GetTrieKeyVal(map.get(), (unsigned char *) s, size);
	}
	bool containsKey(char *s, int size) { // 是否存在该键
		return TrieExistKeyVal(map.get(), (unsigned char *) s, size);
	}
	int clear() { // 清空
		return ClearTrie(map.get());
	}
	bool empty() { // 是否为空
		return TrieEmpty(map.get());
	}

	STRIE *getStrie() {
		return map.get();
	}

	template<typename list_T> ArrayList<list_T> getValList() {
		ArrayList<list_T> result;

		/*将所有值汇总成一个指定类型的列表*/

		Stack<STRIE> stack;

		stack.push(map.get());

		while (stack.empty()) {
			STRIE *temp = stack.pop();
			if (temp != NULL) {
				for (int i = 0; i < 256; i++) {
					stack.push(temp->child[i]);
				}
				if (map.get()->isexist == 1) {
					result.add(cast_class(list_T, temp->data));
				}
			}
		}

		return result;
	}
};