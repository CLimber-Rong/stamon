/*
	Name: ArrayList.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 28/07/23 17:10
	Description: 动态数组
*/

#pragma once

#include "IArrayList.hpp"

namespace stamon::stdc {
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

	void reallocList(int len) {
		if (len < (cache_length / 4)) {
			// 小于缓存的四分之一后，缓存才能缩小至二分之一
			// 之所以要小于四分之一的容量才缩小二分之一，是为了防止出现临界震荡

			T *temp = new T[cache_length / 2];
			for (int i = 0; i < len; i++) {
				temp[i] = move(cache[i]);
				// 尽可能地复制
			}

			destroyCache(); // 销毁原来的cache

			cache = temp;
			cache_length = cache_length / 2;
		} else if (len >= cache_length) {
			// 扩容至两倍

			T *temp = new T[cache_length * 2];
			for (int i = 0; i < length; i++) {
				temp[i] = move(cache[i]);
			}

			destroyCache(); // 销毁原来的cache

			cache = temp;
			cache_length = cache_length * 2;
		}
		// 如果不满足缩小条件，也不满足扩容条件，就什么都不做
	}

	void destroyCache() {
		if (cache != NULL) {
			delete[] cache;
		}
	}

	void deepCopy(const ArrayList<T> &list) {
		// 深拷贝
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

	void shallowCopy(ArrayList<T> &&list) {
		// 浅拷贝
		cache = list.cache;
		cache_length = list.cache_length;
		length = list.length;
		list.cache = NULL;
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
		deepCopy(list);
	}

	ArrayList(ArrayList<T> &&list) {
		shallowCopy(move(list));
	}

	ArrayList<T> &operator=(const ArrayList<T> &list) {
		destroyCache();
		deepCopy(list);
		return *this;
	}

	ArrayList<T> &operator=(ArrayList<T> &&list) {
		destroyCache();
		shallowCopy(move(list));
		return *this;
	}

	void add(T value) {
		reallocList(length + 1); // 重新分配内存
		cache[length] = move(value);
		length++;
	} // 末尾添加值

	void insert(int index, T value) {
		reallocList(length + 1); // 重新分配内存

		for (int i = length - 1; i >= index; i--) {
			cache[i + 1] = cache[i];
		}

		cache[index] = move(value);
		length++;
	} // 将value插入到[index]

	void erase(int index) {
		for (int i = index; i < length - 1; i++) {
			cache[i] = cache[i + 1];
		}

		length--;
		reallocList(length); // 重新分配内存
	} // 删除[index]

	T at(int index) const {
		return cache[index];
	} // 获取[index]

	bool empty() const {
		return (length == 0);
	} // 判断是否为空

	void clear() {
		destroyCache(); // 销毁
		cache = new T[1]; // 新建缓冲区
		cache_length = 1;
		length = 0; // 长度清零
	} // 清空列表

	int size() const {
		return length;
	} // 获得元素个数

	ArrayList<T> operator+(ArrayList<T> src) const {
		ArrayList<T> rst = *this;

		for (int i = 0, len = src.size(); i < len; i++) {
			rst.add(move(src[i]));
		}

		return rst;
	}

	ArrayList<T> &operator+=(ArrayList<T> src) {
		return *(this) = *(this) + src;
	}

	T &operator[](int index) {
		return cache[index];
	}

	T operator[](int index) const {
		return cache[index];
	}

	~ArrayList() {
		destroyCache();
	}
};
} // namespace stamon::stdc

namespace stamon {

template<typename T>
using ArrayList = interface::IArrayList<T, stdc::ArrayList<T>>;

} // namespace stamon