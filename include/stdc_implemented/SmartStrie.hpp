/*
	Name: SmartStrie.hpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 15/02/25 20:11
	Description: 封装Strie指针，来防止内存泄漏，利用了引用计数法
*/

#pragma once

#include "stmlib.hpp"
#include "strie.h"

/*
 * 需要留意的是，对于一个名为ptr的指针
 * 如果需要将其指向的内存地址自增，不能使用*ptr++，*ptr++会先将指针自增再解引用
 * 因此正确的做法应该是(*ptr)++或者++*ptr或者*ptr+=1
 */

class SmartStrie {
public:
	int *ref_cnt; // 引用计数器
	STRIE *trie; // 当前指针

	SmartStrie() {
		// 初始化
		ref_cnt = new int; // 初始化计数器
		*ref_cnt = 1; // 初始化时，只有一个SmartPointer拥有这个指针
		trie = InitTrie();
	}

	SmartStrie(const SmartStrie &value) {
		ref_cnt = value.ref_cnt; // 复制计数器
		trie = value.trie; // 复制指针
		(*ref_cnt)++; // 又多了一个SmartStrie指向这个指针
	}

	SmartStrie &operator=(const SmartStrie &value) {
		/*
		 * 将当前SmartStrie重新指定一个新的指针
		 * 就意味着需要抛弃当前的指针，指向新的指针
		 * 因此需要先减去当前指针的计数器，再将value的计数器加一
		 */

		if (&value == this) {
			// 自我赋值，直接返回即可
			return *this;
		}

		(*ref_cnt)--; // 减去当前计数器
		if (*ref_cnt == 0) {
			delete ref_cnt;
			DestroyTrie(trie);
			// 如果已经没有任何SmartStrie指向该指针
			// 那么此ptr可以被视作垃圾指针，需要被删除
		}

		ref_cnt = value.ref_cnt; // 复制value
		trie = value.trie;

		(*ref_cnt)++; // value的计数器加一

		return *this;
	}

	STRIE *get() const {
		// 获取指针
		return trie;
	}

	~SmartStrie() {
		// 当前计数器需要减一
		(*ref_cnt)--;
		if (*ref_cnt == 0) {
			delete ref_cnt;
			DestroyTrie(trie);
			// 原理同上
		}
	}
};