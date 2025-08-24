#pragma once 

#include <stdint.h>

template <typename T>
inline void SET_BIT(T &var, int pos)
{
	static_assert(std::is_unsigned<T>::value, "SET_BIT only works on unsigned integers");
	static_assert(sizeof(T) <= 8, "SET_BIT only supports up to 64-bit types");
	// if constexpr (std::is_constant_evaluated())//C++20才支持
	// {
	// 	static_assert(pos >= 0 && pos < (int)(sizeof(T) * 8), "Bit position out of range");
	// }
	var |= (T(1) << pos);
}

template <typename T>
inline void CLR_BIT(T &var, int pos)
{
	var &= ~(T(1) << pos);
}

template <typename T>
inline void TGL_BIT(T &var, int pos)
{
	var ^= (T(1) << pos);
}

template <typename T>
inline bool READ_BIT(T var, int pos)
{
	return (var >> pos) & 1;
}