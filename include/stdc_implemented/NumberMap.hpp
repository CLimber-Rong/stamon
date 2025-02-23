/*
	Name: NumberMap.hpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 23/08/23 14:20
	Description: 整数map
*/

#pragma once

#include "ArrayList.hpp"
#include "ByteMap.hpp"
#include "Stack.hpp"
#include "stmlib.hpp"

template<typename T> class NumberMap {
	ByteMap<T> map;

public:
	NumberMap() {}
	int put(int s, T *data) { // 设置键值
		return map.put((char *) &s, sizeof(int), data);
	}
	int del(int s) { // 删除键值
		return map.del((char *) &s, sizeof(int));
	}
	T *get(int s) { // 获取值
		return map.get((char *) &s, sizeof(int));
	}
	bool containsKey(int s) { // 是否存在该键
		return map.containsKey((char *) &s, sizeof(int));
	}
	int clear() { // 清空
		return map.clear();
	}
	bool empty() { // 是否为空
		return map.empty();
	}

	STRIE *getStrie() {
		return map.getStrie();
	}

	template<typename list_T> ArrayList<list_T> getValList() {
		ArrayList<list_T> result;

		/*将所有值汇总成一个指定类型的列表*/

		Stack<STRIE> stack;

		stack.push(map.getStrie());

		while (stack.empty()) {
			STRIE *temp = stack.pop();
			if (temp != NULL) {
				for (int i = 0; i < 256; i++) {
					stack.push(temp->child[i]);
				}
				if (temp->isexist == 1) {
					result.add(cast_class(list_T, temp->data));
				}
			}
		}

		return result;
	}
};