/** Copyright 2017 Politecnico di Milano
 * Developed by: Stefano Cherubin
 * PhD student, Politecnico di Milano
 * <first_name>.<family_name>@polimi.it
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef FIXED_POINT_UTILS_HPP
#define FIXED_POINT_UTILS_HPP

#include <cstdint>

// ----------------------------------------------------------------------------
// 32 VS 64 BIT ARCHITECTURE CHECK
// ----------------------------------------------------------------------------

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define _IS64bit 1
#else
#define _IS32bit 1
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define _IS64bit 1
#else
#define _IS32bit 1
#endif
#endif

#ifdef _MIOSIX
#  define _FIXED_POINT_REDEFINE_INT_TYPES_ true
#else
#  define _FIXED_POINT_REDEFINE_INT_TYPES_ false
#endif // miosix specific case

//-----------------------------------------------------------------------------
// LENGHT RELATED TEMPLATE HELPERS
//-----------------------------------------------------------------------------

// This is a dummy type which means there are too many bits in the data type
struct TooManyBits;

// \return smallest signed integer type with at least BITS bit width
template <uint16_t BITS>
struct get_int_with_length
{
	// The default is that an integer with N bits can be represented
	// by an integer with N+1 bits
	typedef typename get_int_with_length<BITS+1>::RESULT RESULT;
};

template <> struct get_int_with_length<8>   { typedef int8_t RESULT; };

template <> struct get_int_with_length<16>  { typedef int16_t RESULT; };

template <> struct get_int_with_length<32>  { typedef int32_t RESULT; };

template <> struct get_int_with_length<64>  { typedef int64_t RESULT; };

#ifdef _IS64bit
template <> struct get_int_with_length<128> { typedef __int128 RESULT; };
#endif

// early stop
template <> struct get_int_with_length<256> { typedef TooManyBits RESULT; };

// avoid compilation segfault
template <> struct get_int_with_length<65535> { typedef TooManyBits RESULT; };

// \return smallest unsigned integer type with at least BITS bit width
template <uint16_t BITS>
struct get_uint_with_length
{
	// The default is that an integer with N bits can be represented
	// by an integer with N+1 bits
	typedef typename get_uint_with_length<BITS+1>::RESULT RESULT;
};

template <> struct get_uint_with_length<8>   { typedef uint8_t RESULT; };

template <> struct get_uint_with_length<16>  { typedef uint16_t RESULT; };

template <> struct get_uint_with_length<32>  { typedef uint32_t RESULT; };

template <> struct get_uint_with_length<64>  { typedef uint64_t RESULT; };

#ifdef _IS64bit
template <> struct get_uint_with_length<128> { typedef __uint128_t RESULT; };
#endif

// early stop
template <> struct get_uint_with_length<256> { typedef TooManyBits RESULT; };

// avoid compilation segfault
template <> struct get_uint_with_length<65535> { typedef TooManyBits RESULT; };

//-----------------------------------------------------------------------------
// COMPILATION LOGIC TEMPLATES
//-----------------------------------------------------------------------------

/// Returns X1 if isX1 is true, else return X2
template <uint16_t X1, uint16_t X2, bool isX1>
struct pick_max { static const uint16_t RESULT; }; // undefined

template <uint16_t X1, uint16_t X2>
struct pick_max<X1, X2, true>  { static const uint16_t RESULT = X1; };

template <uint16_t X1, uint16_t X2>
struct pick_max<X1, X2, false> { static const uint16_t RESULT = X2; };

/// Returns X1 if (X1 > X2) else returns X2
template <uint16_t X1, uint16_t X2> struct get_max {
	static const uint16_t RESULT = pick_max<X1, X2, (X1 > X2)>::RESULT;
};

/// Returns X1 if (X1 > X2) else returns X2
template <uint16_t X1, uint16_t X2> struct get_min {
	static const uint16_t RESULT = pick_max<X1, X2, (X1 < X2)>::RESULT;
};

//-----------------------------------------------------------------------------
// CONVERSION TEMPLATES
//-----------------------------------------------------------------------------

template<typename src_t, typename dst_t, uint32_t sha, bool isLeft>
struct convert_fixed_point {
	static dst_t exec(src_t src);
};

template<typename src_t, typename dst_t, uint32_t sha>
struct convert_fixed_point<src_t, dst_t, sha, true> {
	static dst_t exec(src_t src) {
		dst_t intermediate = static_cast<dst_t>(src);
		intermediate <<= sha;
		return intermediate;
	}
};

template<typename src_t, typename dst_t, uint32_t sha>
struct convert_fixed_point<src_t, dst_t, sha, false> {
	static dst_t exec(src_t src) {
		src_t intermediate = src;
		intermediate >>= sha;
		return static_cast<dst_t>(intermediate);
	}
};

#endif /* end of include guard: FIXED_POINT_UTILS_HPP */
