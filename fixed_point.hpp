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

#ifndef FIXED_POINT_HPP
#define FIXED_POINT_HPP

#include <cmath>
#include <iomanip>
#include <ostream>
#include <stdint.h>

#include "fixed_point_utils.hpp"

/// A fixed-point integer type
/** \tparam INT_BITS The number of bits before the radix point
 *  \tparam FRAC_BITS The number of bits after the radix point
 *  \warning INT_BITS and FRAC_BITS must be non-negative, and their sum cannot exceed 64
 *
 *  Fixed point numbers are signed, so fixed_point_t<5,2>, for example, has a
 *  range of -16.00 to +15.75
 *
 *  The internal storage (fixed_point_t::raw) is rounded up to the next highest
 *  power of 2, so a 17 bit number occupies 32 physical bits. Consider that the
 *  multiplication of a 5 bit fixed point (which occupies 8 bits of space) with
 *  an 10 bit fixed point (which occupies 16 bits of space) results in a 15 bit
 *  fixed point which occupies 16 bits of space.
 */
template <uint16_t INT_BITS = 1, uint16_t FRAC_BITS = 15>
struct fixed_point_t
{

	//---------------------------------------------------------------------------
	// type definitions for internal data
	//---------------------------------------------------------------------------

public:
	static const uint16_t integer_length = INT_BITS;
	static const uint16_t fractional_length = FRAC_BITS;
	static const uint16_t bit_width = INT_BITS + FRAC_BITS;

	/// The integer type used internally to store the value
	typedef typename get_int_with_length<INT_BITS + FRAC_BITS>::RESULT raw_t;

protected:
	typedef fixed_point_t<INT_BITS, FRAC_BITS> this_t;

private:
	raw_t raw;

public:
	static const raw_t one  = ((raw_t)1) << (FRAC_BITS - 1);
	static const raw_t zero = ((raw_t)0) << FRAC_BITS;

public:
	//---------------------------------------------------------------------------
	// constructors
	//---------------------------------------------------------------------------

	/// Create a fixed-point with equivalent integer value
	/** For example in 4.12 fixed-point, the number "2" is 0010.000000000000  */
	fixed_point_t(const int8_t value)   : raw(static_cast<raw_t>(value) << FRAC_BITS) {}
	fixed_point_t(const uint8_t value)  : raw(static_cast<raw_t>(value) << FRAC_BITS) {}
	fixed_point_t(const int16_t value)  : raw(static_cast<raw_t>(value) << FRAC_BITS) {}
	fixed_point_t(const uint16_t value) : raw(static_cast<raw_t>(value) << FRAC_BITS) {}
	fixed_point_t(const int32_t value)  : raw(static_cast<raw_t>(value) << FRAC_BITS) {}
	fixed_point_t(const uint32_t value) : raw(static_cast<raw_t>(value) << FRAC_BITS) {}
	fixed_point_t(const int64_t value)  : raw(static_cast<raw_t>(value) << FRAC_BITS) {}
	fixed_point_t(const uint64_t value) : raw(static_cast<raw_t>(value) << FRAC_BITS) {}
	fixed_point_t(const long double value) : raw((raw_t)(value * one)) {}
	fixed_point_t(const double value)      : raw((raw_t)(value * one)) {}
	fixed_point_t(const float value)       : raw((raw_t)(value * one)) {}
	#if _FIXED_POINT_REDEFINE_INT_TYPES_
	fixed_point_t(const int value)         : raw(static_cast<raw_t>(value) << FRAC_BITS) {}
	fixed_point_t(const unsigned int value): raw(static_cast<raw_t>(value) << FRAC_BITS) {}
	#endif

	explicit fixed_point_t() : raw(0) {}

	static this_t createRaw(raw_t data) {
		this_t val;
		val.raw = data;
		return val;
	}

	raw_t getRaw() const {
		return this->raw;
	}

	//---------------------------------------------------------------------------
	// conversion
	//---------------------------------------------------------------------------
public:

this_t convert() const {
	return *this;
}

/// Returns a new fixed-point in a new format which is similar in value to the original
/** This may result in loss of raw if the number of bits for either the
 * integer or fractional part are less than the original. */
template <uint16_t INT_BITS_NEW, uint16_t FRAC_BITS_NEW>
fixed_point_t<INT_BITS_NEW, FRAC_BITS_NEW> convert() const
{
	typedef fixed_point_t<INT_BITS_NEW, FRAC_BITS_NEW> target_t;
	typedef typename target_t::raw_t target_raw_t;
	return target_t::createRaw(
		convert_fixed_point<
			raw_t,
			target_raw_t,
			get_max<FRAC_BITS_NEW, FRAC_BITS>::RESULT - get_min<FRAC_BITS_NEW, FRAC_BITS>::RESULT,
			(FRAC_BITS_NEW > FRAC_BITS)
		>:: exec(raw));
}

/// Returns a new fixed-point that reinterprets the binary raw.
/** \warning This should be used sparingly since returns a number whos
 * value is not the necessarily same.
 * \note To just move the radix point, rather use LeftShift or RightShift. */
template <uint16_t INT_BITS_NEW, uint16_t FRAC_BITS_NEW>
fixed_point_t<INT_BITS_NEW, FRAC_BITS_NEW> reinterpret() const
{
	typedef fixed_point_t<INT_BITS_NEW, FRAC_BITS_NEW> target_t;
	return target_t::createRaw(raw);
}

	//---------------------------------------------------------------------------
	// arithmetic operators
	//---------------------------------------------------------------------------
public:

this_t operator+(const this_t& value) const
{
	return this_t::createRaw(this->getRaw() + value.getRaw());
}

template <uint16_t INT_BITS2, uint16_t FRAC_BITS2>
this_t operator+(const fixed_point_t<INT_BITS2, FRAC_BITS2>& value) const
{
	this_t op2 = value.template convert<INT_BITS, FRAC_BITS>();
	return this_t::createRaw(this->getRaw() + op2.getRaw());
}

this_t& operator+=(const this_t& value)
{
	raw += value.getRaw();
	return *this;
}

template <uint16_t INT_BITS2, uint16_t FRAC_BITS2>
this_t& operator+=(const fixed_point_t<INT_BITS2, FRAC_BITS2>& value)
{
	this_t op2 = value.template convert<INT_BITS, FRAC_BITS>();
	return *this += op2;
}

template <typename other_t>
this_t operator+(const other_t& value) const
{
	return *this + this_t(value);
}

template <typename other_t>
this_t& operator+=(const other_t& value)
{
	return *this += this_t(value);
}

this_t& operator++(int)
{
	raw += one;
	return *this;
}

this_t& operator++()
{
	raw += one;
	return *this;
}


/// Inverse operator
this_t operator-() const
{
	return this_t::createRaw(-raw);
}


template <uint16_t INT_BITS2, uint16_t FRAC_BITS2>
this_t operator-(const fixed_point_t<INT_BITS2, FRAC_BITS2>& value) const
{
	this_t op2 = value.template convert<INT_BITS, FRAC_BITS>();
	return this_t::createRaw(getRaw() - op2.getRaw());
}

template <uint16_t INT_BITS2, uint16_t FRAC_BITS2>
this_t& operator-=(const fixed_point_t<INT_BITS2, FRAC_BITS2>& value)
{
	this_t op2 = value.template convert<INT_BITS, FRAC_BITS>();
	raw -= op2.getRaw();
	return *this;
}

this_t& operator--(int)
{
	raw -= one;
	return *this;
}

this_t& operator--()
{
	raw -= one;
	return *this;
}

template <typename other_t>
this_t operator-(const other_t& value) const
{
	return *this - this_t(value);
}

template <typename other_t>
this_t& operator-=(const other_t& value)
{
	return *this -= this_t(value);
}


/// Multiplication with another fixed-point
template <uint16_t INT_BITS2, uint16_t FRAC_BITS2>
this_t operator*(const fixed_point_t<INT_BITS2, FRAC_BITS2>& value) const
{
	typedef fixed_point_t<INT_BITS + INT_BITS2, FRAC_BITS + FRAC_BITS2> result_t;
	typedef typename result_t::raw_t result_raw_t;
	result_t extended_res = result_t::createRaw(static_cast<result_raw_t>(getRaw()) * static_cast<result_raw_t>(value.getRaw()));
	return extended_res.template convert<INT_BITS, FRAC_BITS>();
}

template <typename other_t>
this_t operator*(const other_t& value) const
{
	return *this * this_t(value);
}

this_t& operator*=(const this_t& value) {
	const auto tmp = *this * value;
	raw = tmp.template convert<INT_BITS, FRAC_BITS>().getRaw();
	return *this;
}

template <uint16_t INT_BITS2, uint16_t FRAC_BITS2>
this_t& operator*=(const fixed_point_t<INT_BITS2, FRAC_BITS2>& value)
{
	const auto tmp = *this * value;
	raw = tmp.template convert<INT_BITS, FRAC_BITS>().getRaw();
	return *this;
}

template <typename other_t>
this_t& operator*=(const other_t& value)
{
	return *this *= this_t(value);
}


/// Divide operator
template <uint16_t INT_BITS2, uint16_t FRAC_BITS2>
this_t operator/(const fixed_point_t<INT_BITS2, FRAC_BITS2>& divisor) const
{
	const uint16_t I_RES = INT_BITS + FRAC_BITS2;
	const uint16_t F_RES = INT_BITS2 + FRAC_BITS;
	typedef fixed_point_t<I_RES, F_RES> result_t;
	typedef typename result_t::raw_t result_raw_t;
	// Expand the dividend so we don't lose resolution
	result_raw_t intermediate = static_cast<result_raw_t>(raw);
	// Shift the dividend. FRAC_BITS2 cancels with the fractional bits in
	// divisor, and INT_BITS2 adds the required resolution.
	intermediate <<= (F_RES - 1); //FRAC_BITS + INT_BITS2 - signum;
	intermediate /= divisor.getRaw();
	result_t tmp = result_t::createRaw(intermediate);
	return tmp.template convert<INT_BITS, FRAC_BITS>();
}

template <typename other_t>
this_t operator/(const other_t& value) const
{
	return *this / this_t(value);
}

this_t& operator/=(const this_t& value) {
	const auto tmp = *this / value;
	raw = tmp.getRaw();
	return *this;
}

template <uint16_t INT_BITS2, uint16_t FRAC_BITS2>
this_t& operator/=(const fixed_point_t<INT_BITS2, FRAC_BITS2>& value)
{
	const auto tmp = *this / value;
	raw = tmp.template convert<INT_BITS, FRAC_BITS>().getRaw();
	return *this;
}

template <typename other_t>
this_t& operator/=(const other_t& value)
{
	return *this /= this_t(value);
}

//---------------------------------------------------------------------------
// logic operators
//---------------------------------------------------------------------------
public:

// FIXME handle case of signum bit overritten
template <uint16_t INT_BITS2, uint16_t FRAC_BITS2>
bool operator < (const fixed_point_t<INT_BITS2, FRAC_BITS2>& other) const
{
	typedef fixed_point_t<INT_BITS2, FRAC_BITS2> other_t;
	other_t this_converted = this->template convert<INT_BITS2,FRAC_BITS2>();
	this_t other_converted = other.template convert<INT_BITS,FRAC_BITS>();
	bool res_1 = *this < other_converted && this_converted < other;
	if (res_1) {
		return true;
	}
	bool res_2 = *this > other_converted && this_converted > other;
	res_2 = res_2  || (*this == other_converted && this_converted == other);
	if (res_2) {
		return false;
	}
	if (INT_BITS > INT_BITS2) {
		return *this < other_converted;
	} else if (INT_BITS > INT_BITS2) {
		return this_converted < other;
	} else if (FRAC_BITS < FRAC_BITS2) {
		return this_converted < other;
	} else {
		return *this < other_converted;
	}
}

template <uint16_t INT_BITS2, uint16_t FRAC_BITS2>
bool operator == (const fixed_point_t<INT_BITS2, FRAC_BITS2>& other) const
{
	typedef fixed_point_t<INT_BITS2, FRAC_BITS2> other_t;
	other_t this_converted = this->template convert<INT_BITS2,FRAC_BITS2>();
	fixed_point_t other_converted = other.template convert<INT_BITS,FRAC_BITS>();
	return (*this == other_converted && this_converted == other);
}

template <uint16_t INT_BITS2, uint16_t FRAC_BITS2>
bool operator != (const fixed_point_t<INT_BITS2, FRAC_BITS2>& other) const
{
	return !(*this == other);
}

template <uint16_t INT_BITS2, uint16_t FRAC_BITS2>
bool operator > (const fixed_point_t<INT_BITS2, FRAC_BITS2>& other) const
{
	typedef fixed_point_t<INT_BITS2, FRAC_BITS2> other_t;
	other_t this_converted = this->template convert<INT_BITS2,FRAC_BITS2>();
	this_t other_converted = other.template convert<INT_BITS,FRAC_BITS>();
	bool res_1 = *this > other_converted && this_converted > other;
	if (res_1) {
		return true;
	}
	bool res_2 = *this < other_converted && this_converted < other;
	res_2 = res_2  || (*this == other_converted && this_converted == other);
	if (res_2) {
		return false;
	}
	// resolved at compile-time
	if (INT_BITS > INT_BITS2) {
		return *this > other_converted;
	} else if (INT_BITS > INT_BITS2) {
		return this_converted > other;
	} else if (FRAC_BITS < FRAC_BITS2) {
		return this_converted > other;
	} else {
		return *this > other_converted;
	}
}

template <uint16_t INT_BITS2, uint16_t FRAC_BITS2>
bool operator <= (const fixed_point_t<INT_BITS2, FRAC_BITS2>& other) const
{
	return !(*this > other);
}

template <uint16_t INT_BITS2, uint16_t FRAC_BITS2>
bool operator >= (const fixed_point_t<INT_BITS2, FRAC_BITS2>& other) const
{
	return !(*this < other);
}

bool operator < (const this_t& value) const
{
	return raw < value.getRaw();
}

bool operator > (const this_t& value) const
{
	return raw > value.getRaw();
}

bool operator == (const this_t& value) const
{
	return raw == value.getRaw();
}

bool operator != (const this_t& value) const
{
	return raw != value.getRaw();
}

bool operator <= (const this_t& value) const
{
	return ! (*this > value);
}

bool operator >= (const this_t& value) const
{
	return ! (*this < value);
}

template <typename other_t>
bool operator < (const other_t& other) const
{
	return *this < this_t(other);
}

template <typename other_t>
bool operator > (const other_t& other) const
{
	return *this > this_t(other);
}

template <typename other_t>
bool operator == (const other_t& other) const
{
	return *this == this_t(other);
}

template <typename other_t>
bool operator != (const other_t& other) const
{
	return *this != this_t(other);
}

template <typename other_t>
bool operator <= (const other_t& other) const
{
	return *this <= this_t(other);
}

template <typename other_t>
bool operator >= (const other_t& other) const
{
	return *this >= this_t(other);
}


//---------------------------------------------------------------------------
// assignment operator
//---------------------------------------------------------------------------

template<typename other_t>
this_t& operator=(const other_t& value)
{
	raw = this_t(value).getRaw();
	return *this;
}

template<uint16_t INT_BITS2, uint16_t FRAC_BITS2>
this_t& operator=(const fixed_point_t<INT_BITS2, FRAC_BITS2>& value)
{
	raw = value.template convert<INT_BITS, FRAC_BITS>().getRaw();
	return *this;
}

this_t& operator=(const this_t& value)
{
	raw = value.getRaw();
	return *this;
}


//---------------------------------------------------------------------------
// pretty print
//---------------------------------------------------------------------------

/// Write to an output stream
std::ostream& emit(std::ostream& os) const
{
	auto old_precision = os.precision();
	auto old_flags = os.flags();
	os << std::fixed << std::setprecision((FRAC_BITS * 3 + 9) / 10)
		<< getValueF() << std::setprecision(old_precision);
	os.flags(old_flags);
	return os;
}


//---------------------------------------------------------------------------
// accessors
//---------------------------------------------------------------------------

/// Get the value as a floating point
float getValueF() const { return static_cast<float>(raw)/one; }

/// Get the value as a floating point double precision
double getValueFD() const { return static_cast<double>(raw)/one; }

/// Get the value as a floating point quadruple precision
long double getValueFLD() const { return static_cast<long double>(raw)/one; }

/// Get the value truncated to an integer
raw_t getValue() const { return static_cast<raw_t>(raw >> FRAC_BITS); }

/// Get the closest integer value
raw_t round() const { return static_cast<raw_t>(std::round(getValueF())); }

//---------------------------------------------------------------------------
// conversion
//---------------------------------------------------------------------------

#if _FIXED_POINT_REDEFINE_INT_TYPES_
/// convert to int
explicit operator int() const { return static_cast<int>(getValue()); }

/// convert to unsigned int
explicit operator unsigned int() const { return static_cast<unsigned int>(getValue()); }
#endif

/// convert to int16_t
explicit operator int16_t() const { return static_cast<int16_t>(getValue()); }

/// convert to int32_t
explicit operator int32_t() const { return static_cast<int32_t>(getValue()); }

/// convert to int64_t
explicit operator int64_t() const { return static_cast<int64_t>(getValue()); }

/// convert to uint16_t
explicit operator uint16_t() const { return static_cast<uint16_t>(getValue()); }

/// convert to uint32_t
explicit operator uint32_t() const { return static_cast<uint32_t>(getValue()); }

/// convert to uint64_t
explicit operator uint64_t() const { return static_cast<uint64_t>(getValue()); }

/// convert to float
explicit operator float() const { return getValueF(); }

/// convert to double
explicit operator double() const { return getValueFD(); }

/// convert to long double
explicit operator long double() const { return getValueFLD(); }

template<uint16_t INT_BITS2, uint16_t FRAC_BITS2>
explicit operator fixed_point_t<INT_BITS2, FRAC_BITS2>() const {
	return this->template convert<INT_BITS2,FRAC_BITS2>();
}
};

#include "fixed_point_external_operators.hpp"

#endif /* end of include guard: FIXED_POINT_HPP */
