/*
	Name: AllocatorAdapter
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 25/06/26 20:47
	Description: 分配器适配器，用于适配std::allocator
*/

#pragma once

#include "Allocator.hpp"
#include <cstddef>
#include <memory>
#include <stdexcept>

namespace stamon::stdcpp {

template<typename T> class AllocatorAdapter {
	Allocator *allocator;

public:
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using propagate_on_container_copy_assignment = std::true_type;
	using propagate_on_container_move_assignment = std::true_type;
	using propagate_on_container_swap = std::true_type;
	using is_always_equal = std::false_type;

	explicit AllocatorAdapter(Allocator *allocator) noexcept
		: allocator(allocator) {
	}

	// 让AllocatorAdapter<U>能访问私有成员
	template<typename U> friend class AllocatorAdapter;

	// 跨类型拷贝构造
	template<typename U>
	AllocatorAdapter(const AllocatorAdapter<U> &other) noexcept
		: allocator(other.getAllocator()) {
	}

	AllocatorAdapter(const AllocatorAdapter &other) noexcept = default;
	AllocatorAdapter(AllocatorAdapter &&other) noexcept = default;
	AllocatorAdapter &operator=(const AllocatorAdapter &) noexcept = default;
	AllocatorAdapter &operator=(AllocatorAdapter &&) noexcept = default;
	~AllocatorAdapter() = default;

	[[nodiscard]] T *allocate(size_type n) {
		if (n > max_size()) {
			throw std::bad_array_new_length();
		}

		void *ptr = allocator->allocMem(n * sizeof(T));

		return (T *) (ptr);
	}

	void deallocate(T *ptr, size_type n) noexcept {
		allocator->deallocMem((void *) (ptr));
	}

	size_type max_size() const noexcept {
		return (size_type) (-1) / sizeof(T);
	}

	template<typename U>
	bool operator==(const AllocatorAdapter<U> &other) const noexcept {
		return allocator == other.getAllocator();
	}

	template<typename U>
	bool operator!=(const AllocatorAdapter<U> &other) const noexcept {
		return allocator != other.getAllocator();
	}

	Allocator *getAllocator() const noexcept {
		return allocator;
	}

	AllocatorAdapter select_on_container_copy_construction() const noexcept {
		return *this;
	}
};

} // namespace stamon::stdcpp

namespace stamon {
template<typename T> using AllocatorAdapter = stdcpp::AllocatorAdapter<T>;
} // namespace stamon