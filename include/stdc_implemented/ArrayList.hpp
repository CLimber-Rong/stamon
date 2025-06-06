/*
	Name: ArrayList.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 28/07/23 17:10
	Description: 动态数组
*/

#pragma once

#include "stmlib.hpp"

template<typename T> class ArrayList {
	/*
	 * 采用双倍扩容法
	 * 设立了cache_length和length
	 * cache_length代表缓冲区长度，即当前最多能存储多少元素
	 * length代表当前已经存储了多少元素
	 * 不难发现，剩余的空间可以用(cache_length-length)来表示
	 * 我们规定：当缓冲区已经被占满，将缓冲区翻倍扩容
	 * 我们规定：当实际占用的容量小于缓冲区容量的二分之一时，将缓冲区缩小为原来的一半
	 * 这样，我们将ArrayList的分配次数从线性级将为对数级
	 */
	
	T *cache = NULL;
	int cache_length = 0;
	int length = 0;

	void realloc_list(int len) {
		if (len < (cache_length / 2)) {
			// 缩小至一半

			T *temp = new T[cache_length / 2];
			for (int i = 0; i < len; i++) {
				temp[i] = cache[i];
				// 尽可能地复制
			}

			destroy_cache(); // 销毁原来的cache

			cache = temp;
			cache_length = cache_length / 2;
		} else if (len >= cache_length) {
			// 扩容至两倍

			T *temp = new T[cache_length * 2];
			for (int i = 0; i < length; i++) {
				temp[i] = cache[i];
			}

			destroy_cache(); // 销毁原来的cache

			cache = temp;
			cache_length = cache_length * 2;
		}
		// 如果不满足缩小条件，也不满足扩容条件，就什么都不做
	}

	void destroy_cache() {
		if (cache != NULL) {
			delete[] cache;
		}
	}

public:
	ArrayList() {
		cache = new T[1];
		cache_length = 1;
		length = 0;
	}

	ArrayList(int size) {
		if (size == 0) {
			cache_length = 1;
		} else {
			cache_length = size;
		}
		cache = new T[cache_length];
		length = size;
	}

	ArrayList(const ArrayList<T> &list) {
		if (list.size() == 0) {
			// 缓冲区至少要有一个元素
			cache_length = 1;
		} else {
			cache_length = list.length;
		}

		cache = new T[cache_length];

		for (int i = 0; i < list.size(); i++) {
			cache[i] = list[i];
		}

		length = list.length;
	}

	ArrayList(ArrayList<T> &&list) {
		cache = list.cache;
		cache_length = list.cache_length;
		length = list.length;
		list.cache = NULL;
	}

	ArrayList<T> &operator=(const ArrayList<T> &list) {
		destroy_cache();

		if (list.size() == 0) {
			// 缓冲区至少要有一个元素
			cache_length = 1;
		} else {
			cache_length = list.length;
		}

		cache = new T[cache_length];

		for (int i = 0; i < list.size(); i++) {
			cache[i] = list[i];
		}

		length = list.length;

		return *this;
	}

	ArrayList<T> &operator=(ArrayList<T> &&list) {
		destroy_cache();

		cache = list.cache;
		cache_length = list.cache_length;
		length = list.length;
		list.cache = NULL;

		return *this;
	}

	void add(const T &value) {
		realloc_list(length + 1); // 重新分配内存
		cache[length] = value;
		length++;
	} // 末尾添加值

	void insert(int index, const T &value) {
		realloc_list(length + 1); // 重新分配内存

		for (int i = length - 1; i >= index; i--) {
			cache[i + 1] = cache[i];
		}

		cache[index] = value;
		length++;
	} // 将value插入到[index]

	void erase(int index) {
		for (int i = index; i < length - 1; i++) {
			cache[i] = cache[i + 1];
		}

		realloc_list(length - 1); // 重新分配内存
		length--;
	} // 删除[index]

	T at(int index) const {
		return cache[index];
	} // 获取[index]

	bool empty() const {
		return (length == 0);
	} // 判断是否为空

	void clear() {
		destroy_cache(); // 销毁
		cache = new T[1]; // 新建缓冲区
		cache_length = 1;
		length = 0; // 长度清零
	} // 清空列表

	int size() const {
		return length;
	} // 获得元素个数

	ArrayList<T> operator+(ArrayList<T> src) {
		ArrayList<T> rst = *this;

		for (int i = 0, len = src.size(); i < len; i++) {
			rst.add(src[i]);
		}

		return rst;
	}

	ArrayList<T> operator+=(ArrayList<T> src) {
		return *(this) = *(this) + src;
	}

	T &operator[](int index) {
		return cache[index];
	}

	T operator[](int index) const {
		return cache[index];
	}

	~ArrayList() {
		destroy_cache();
	}
};