/*
	Name: String.hpp
	Copyright: Apache2.0
	Author: GusemFowage, CLimber-Rong
	Date: 29/07/23 12:59
	Description: 跨平台类型定义库
*/

template<class T, T V>
struct constant_value {
	static constexpr T value = V;
};

using true_type = constant_value<bool, true>;
using false_type = constant_value<bool, false>;

//以下宏定义需要填写对应类型
#define INT8
#define INT16
#define INT32
#define INT64
#define UINT8
#define UINT16
#define UINT32
#define UINT64

// 可以用于去除 符号 限定
// 以及 无符号 限定
// 可以用于转换 类型至相应长度的类型(与 union 配合使用)

//以下部分无需实现

template<size_t N, bool sign>
struct bytes_get{
	typedef void type;
};
template<>
struct bytes_get<1, true>{typedef INT8 type;};
template<>
struct bytes_get<2, true>{typedef INT16 type;};
template<>
struct bytes_get<4, true>{typedef INT32 type;};
template<>
struct bytes_get<8, true>{typedef INT64 type;};
// struct bytes_get<16, true>{typedef int8_t type;};
template<>
struct bytes_get<1, false>{typedef UINT8 type;};
template<>
struct bytes_get<2, false>{typedef UINT16 type;};
template<>
struct bytes_get<4, false>{typedef UINT32 type;};
template<>
struct bytes_get<8, false>{typedef UINT64 type;};

using len_t = bytes_get<sizeof(size_t), false>::type;
//等价于#define len_t bytes_get<sizeof(size_t), false>::type

// 这件事情应该由标准库实现，但是为了兼容性，这里还是自己实现
using nullptr_t = decltype(nullptr);

// 仅仅只是封装了指针
// 在C++中应当避免出现直接裸指针
// 注：
// 	可以使用智能指针，但由于项目设计伊始为了可移植性，
// 	未导入智能指针，故此处仅仅使用封装指针
template<class T>
struct pointer {
	typedef T* 
		ptr_t;
	typedef T& 
		ref_t;
	typedef const T& 
		const_reference;
	typedef T 
		value_type;
	typedef bytes_get<sizeof(void*), false>::type 
		dif_t;

	pointer() noexcept;
	pointer(nullptr_t) noexcept;
	pointer(ptr_t p) noexcept;
	const_reference operator*() const noexcept;
	ref_t operator*() noexcept;
	pointer operator->() const noexcept;
	dif_t operator-(const pointer& other) const noexcept;
	ref_t operator[](dif_t i) noexcept;
	const_reference operator[](dif_t i) const noexcept;
	operator bool() const noexcept;
	operator ptr_t() const noexcept;
	ptr_t get() const noexcept;
protected:	// 可在这的基础上封装智能指针
	ptr_t ptr;
};

// 判断是否是指针
// 此功能还未使用，但项目优化有考虑使用
template<class T>
struct is_pointer : false_type{};
template<class T>
struct is_pointer<pointer<T>> : true_type{};
template<class T>
struct is_pointer<T*> : true_type{};