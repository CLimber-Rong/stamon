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
#include "memory"
#include "unordered_map"

namespace stamon::stdcpp {

template<typename KeyType, typename ValType> class HashMap {
	// 采用智能指针，使用引用传递

	struct HashTool {
		size_t operator()(const KeyType &p) const { // 别忘记const
			return (size_t) toHash(p);
		}
	};

	std::shared_ptr<std::unordered_map<KeyType, ValType, HashTool>> mapptr;

public:
	HashMap()
		: mapptr(std::make_shared<
				  std::unordered_map<KeyType, ValType, HashTool>>()) {
	}

	void put(const KeyType &key, ValType val) {
		(*mapptr)[key] = move(val);
	}

	ValType get(const KeyType &key) const {
		return (*mapptr)[key];
	}

	void del(const KeyType &key) {
		mapptr->erase(key);
	}

	bool exist(const KeyType &key) const {
		return mapptr->find(key) != mapptr->end();
	}

	ArrayList<KeyType> getKeyList() const {
		// 获取一个由所有键组成的列表
		ArrayList<KeyType> rst;
		for (const auto &pair : *mapptr) {
			rst.add(pair.first);
		}
		return rst;
	}

	ArrayList<ValType> getValList() const {
		ArrayList<ValType> rst;
		for (const auto &pair : *mapptr) {
			rst.add(pair.second);
		}
		return rst;
	}
};

} // namespace stamon::stdcpp

namespace stamon {

template<typename KeyType, typename ValType>
using HashMap = interface::IHashMap<KeyType, ValType,
		stdcpp::HashMap<KeyType, ValType>>;

}