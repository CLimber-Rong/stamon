/*
	Name: HashMap.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 07/09/25 13:45
	Description: 哈希表
*/
#pragma once

#include "ArrayList.hpp"
#include "BasicPlatform.hpp"
#include "EasySmartPtr.hpp"
#include "IHashMap.hpp"

namespace stamon::stdc {

template<typename KeyType, typename ValType> class HashMapBucket {
	// 哈希表的桶数组定义

public:
	class Chain {
		// 采用链地址法，这里是链的定义
	public:
		class DataPair {
			// 键值对的定义
		public:
			KeyType key;
			ValType val;
			DataPair() = default;
			DataPair(KeyType k, ValType v)
				: key(move(k))
				, val(move(v)) {
			}
		};
		ArrayList<DataPair> chain;
	};

	Chain *bucket; // 桶数组指针
	unsigned int element_size; // 元素数量
	unsigned int bucket_size; // 桶数量
	constexpr static unsigned int init_capacity = 8; // 初始化容量

	HashMapBucket()
		: element_size(0)
		, bucket_size(init_capacity)
		, bucket(new Chain[init_capacity]) {
	}

	void rehash(Chain *dst, unsigned int size) {
		// 重映射到容量为size的dst
		for (unsigned int i = 0; i < bucket_size; i++) {
			// 遍历每个桶

			ArrayList<typename Chain::DataPair> &chain = bucket[i].chain;

			for (unsigned int j = 0; j < bucket[i].chain.size(); j++) {
				// 遍历每条链
				unsigned int index = toHash(move(chain[j].key)) % size;
				dst[index].chain.add(chain[j]);
			}
		}
	}

	void expand() {
		// 扩容至两倍
		Chain *temp = new Chain[bucket_size * 2]; // 新的桶数组
		rehash(temp, bucket_size * 2); // 重映射
		bucket_size *= 2;
		delete[] bucket;
		bucket = temp;
	}

	void reduce() {
		// 缩小至二分之一，步骤基本同上
		Chain *temp = new Chain[bucket_size / 2];
		rehash(temp, bucket_size / 2);
		bucket_size /= 2;
		delete[] bucket;
		bucket = temp;
	}

	~HashMapBucket() {
		delete[] bucket;
	}
};

template<typename KeyType, typename ValType> class HashMap {
	// 采用智能指针，使用引用传递
	using Bucket = HashMapBucket<KeyType, ValType>;
	EasySmartPtr<Bucket> bucket;

public:
	HashMap()
		: bucket(new Bucket) {
	}

	void put(const KeyType &key, ValType val) {
		// 设置键值对
		if (bucket->element_size >= bucket->bucket_size) {
			// 元素容量大等于总容量的一半，扩容
			bucket->expand();
		}

		unsigned int index = toHash(key) % bucket->bucket_size;
		ArrayList<typename Bucket::Chain::DataPair> &chain
				= bucket->bucket[index].chain; // 获取当前桶的链

		for (unsigned int i = 0, len = chain.size(); i < len; i++) {
			// 遍历当前链，如果该键值对之前已被设置，则直接更新值即可
			if (chain[i].key == key) {
				chain[i].val = move(val);
				return;
			}
		}

		// 没找到键值对，则将键值对添加至末尾
		chain.add(typename Bucket::Chain::DataPair(key, move(val)));

		bucket->element_size++;
	}

	ValType get(const KeyType &key) const {
		// 根据键获取值
		unsigned int index = toHash(key) % bucket->bucket_size;
		ArrayList<typename Bucket::Chain::DataPair> &chain
				= bucket->bucket[index].chain;

		// 遍历搜索除最后一个键值对之外的所有键值对
		for (unsigned int i = 0, len = chain.size() - 1; i < len; i++) {
			if (chain[i].key == key) {
				// 匹配到键值对
				return chain[i].val;
			}
		}

		// 都没匹配到，那么只有可能是最后一个键值对
		return chain[chain.size() - 1].val;
	}

	void del(const KeyType &key) {
		// 根据键删除键值对
		unsigned int index = toHash(key) % bucket->bucket_size;
		ArrayList<typename Bucket::Chain::DataPair> &chain
				= bucket->bucket[index].chain;

		for (unsigned int i = 0, len = chain.size(); i < len; i++) {
			if (chain[i].key == key) {
				// 匹配到键值对
				chain.erase(i);
				return;
			}
		}

		bucket->element_size--;

		if (bucket->element_size <= bucket->bucket_size / 4) {
			// 键值对数量不超过桶数量的四分之一，则桶缩小至二分之一
			// 之所以要小于四分之一的容量才缩小二分之一，是为了防止出现临界震荡
			bucket->reduce();
		}
	}

	bool exist(const KeyType &key) const {
		// 根据键判断是否存在对应的键值对
		unsigned int index = toHash(key) % bucket->bucket_size;
		ArrayList<typename Bucket::Chain::DataPair> &chain
				= bucket->bucket[index].chain;

		for (unsigned int i = 0, len = chain.size(); i < len; i++) {
			if (chain[i].key == key) {
				// 匹配到键值对
				return true;
			}
		}

		return false;
	}

	ArrayList<KeyType> getKeyList() const {
		// 获取一个由所有键组成的列表
		ArrayList<KeyType> rst;

		for (unsigned int i = 0, i_len = bucket->bucket_size; i < i_len; i++) {
			ArrayList<typename Bucket::Chain::DataPair> &chain
					= bucket->bucket[i].chain;
			for (unsigned int j = 0, j_len = chain.size(); j < j_len; j++) {
				rst.add(chain[j].key);
			}
		}

		return rst;
	}

	ArrayList<ValType> getValList() const {
		// 获取一个由所有值组成的列表
		ArrayList<ValType> rst;

		for (unsigned int i = 0, i_len = bucket->bucket_size; i < i_len; i++) {
			ArrayList<typename Bucket::Chain::DataPair> &chain
					= bucket->bucket[i].chain;
			for (unsigned int j = 0, j_len = chain.size(); j < j_len; j++) {
				rst.add(chain[j].val);
			}
		}

		return rst;
	}
};

} // namespace stamon::stdc

namespace stamon {

template<typename KeyType, typename ValType>
using HashMap = interface::IHashMap<KeyType, ValType,
		stdc::HashMap<KeyType, ValType>>;

}