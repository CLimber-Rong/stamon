/**
 * @author: Gusem Fowage
 * @action: 获取长度所对应的类型
 * @notice: usetool 是个人使用文件夹，请勿随意修改
 */
#include <cstdint>
#include <cstddef>

// 可以用于去除 符号 限定
// 以及 无符号 限定
// 可以用于转换 类型至相应长度的类型(与 union 配合使用)

template<size_t N, bool sign>
struct bytes_get{
	typedef void type;
};
template<>
struct bytes_get<1, true>{typedef int8_t type;};
template<>
struct bytes_get<2, true>{typedef int16_t type;};
template<>
struct bytes_get<4, true>{typedef int32_t type;};
template<>
struct bytes_get<8, true>{typedef int64_t type;};
// struct bytes_get<16, true>{typedef int8_t type;};
template<>
struct bytes_get<1, false>{typedef uint8_t type;};
template<>
struct bytes_get<2, false>{typedef uint16_t type;};
template<>
struct bytes_get<4, false>{typedef uint32_t type;};
template<>
struct bytes_get<8, false>{typedef uint64_t type;};
// struct bytes_get<16, false>{typedef uint128_t type;};

using len_t = bytes_get<sizeof(size_t), false>::type;