/*
	Name: Exception.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 07/06/26 17:35
	Description: 使用代数类型实现
*/

#pragma once

#include "Allocator.hpp"
#include "StamonLib.hpp"

namespace stamon {

template<typename T> struct ResultData {
	T data;

	ResultData(T data)
		: data(move(data)) {
	}

	ResultData(const ResultData &) = delete;
	ResultData(ResultData &&) = default;
	ResultData &operator=(const ResultData &) = delete;
	ResultData &operator=(ResultData &&) = default;
};
template<typename E> struct ResultError {
	E error;

	ResultError(E error)
		: error(move(error)) {
	}

	ResultError(const ResultError &) = delete;
	ResultError(ResultError &&) = default;
	ResultError &operator=(const ResultError &) = delete;
	ResultError &operator=(ResultError &&) = default;
};

template<typename T, typename E> class Result {
public:
	static_assert(Movable<T>, "data-value must be movable");
	static_assert(Movable<E>, "error-value must be movable");

	union {
		T data;
		E error;
	} result;

	bool error_flag;

	Result(ResultData<T> data) {
		error_flag = false;
		PlacementNew<T>(&result, move(data.data));
	}

	Result(ResultError<E> error) {
		error_flag = true;
		PlacementNew<T>(&result, move(error.error));
	}

	// 只能移动不能拷贝
	Result(const Result &) = delete;
	Result(Result &&) = default;
	Result &operator=(const Result &) = delete;
	Result &operator=(Result &&) = default;

	T getData() {
		return result.data;
	}

	E getError() {
		return result.error;
	}

	bool isError() {
		return error_flag;
	}

	~Result() {
		if (error_flag) {
			PlacementDelete<E>(&result);
		} else {
			PlacementDelete<T>(&result);
		}
	}
};

} // namespace stamon

#define TRY(rst, expr) \
	MACRO_START \
	if ((rst = (expr)).isError()) { \
		return stamon::ResultError(rst.getError()); \
	} \
	MACRO_END

#define CATCH(expr) if ((expr).isError())