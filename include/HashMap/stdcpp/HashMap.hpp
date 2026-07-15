/*
	Name: HashMap
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 03/07/26 15:45
	Description: 哈希表的标准C++实现
*/

/*
	Name: HashMap.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 07/09/25 13:45
	Description: 哈希表
*/
#pragma once

#include "Allocator.hpp"
#include "AllocatorAdapter.hpp"
#include "ArrayList.hpp"
#include "ArrayListTrait.hpp"
#include "GlobalConfig.hpp"
#include "HashMapTrait.hpp"
#include "StamonLib.hpp"
#include <functional>
#include <unordered_map>
#include <utility>

namespace stamon::stdcpp {

template<typename Key, typename Val> class HashMap {
	// 采用智能指针，使用引用传递

	struct HashTool {
		size_t operator()(const Key &p) const { // 别忘记const
			return (size_t) toHash(p);
		}
	};

	using allocator_type = AllocatorAdapter<std::pair<const Key, Val>>;

	using map_type = std::unordered_map<Key, Val, HashTool, std::equal_to<Key>,
			allocator_type>;

	map_type map;

public:
	HashMap(Allocator *allocator = &config::DefaultAllocator)
		: map(map_type(allocator_type(allocator))) {
	}

	HashMap &put(const Key &key, Val val) {
		map[key] = move(val);
		return *this;
	}

	Val get(const Key &key) const {
		return map.at(key);
	}

	HashMap &del(const Key &key) {
		map.erase(key);
		return *this;
	}

	bool exist(const Key &key) const {
		return map.find(key) != map.end();
	}

	ArrayList<Key> getKeyList() const {
		// 获取一个由所有键组成的列表
		ArrayList<Key> rst;
		for (const auto &pair : map) {
			rst.add(pair.first);
		}
		return rst;
	}

	ArrayList<Val> getValList() const {
		ArrayList<Val> rst;
		for (const auto &pair : map) {
			rst.add(pair.second);
		}
		return rst;
	}
};

} // namespace stamon::stdcpp

namespace stamon {
template<typename Key, typename Val>
using HashMap = trait::checkHashMapTrait<stdcpp::HashMap<Key, Val>, Key, Val>;
} // namespace stamon