#include <cstdint>
/**
 * @author: Gusem Fowage
 * @action: 获取长度所对应的类型
 */
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