#ifndef UFIXED_POINT_OPERATORS_HPP
#define UFIXED_POINT_OPERATORS_HPP

//---------------------------------------------------------------------------
// External operators
//---------------------------------------------------------------------------

// Make the fixed-point struct  ostream outputtable
template<uint16_t INT_BITS, uint16_t FRAC_BITS>
std::ostream& operator<< (std::ostream &stream,
	const ufixed_point_t<INT_BITS, FRAC_BITS> &fixedPoint)
{
	return fixedPoint.emit(stream);
}

// External arithmetic operators

// uint16_t
template<uint16_t INT_BITS, uint16_t FRAC_BITS>
uint16_t operator+(const uint16_t& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs + static_cast<uint16_t>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
uint16_t operator*(const uint16_t& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs * static_cast<uint16_t>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
uint16_t operator-(const uint16_t& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs - static_cast<uint16_t>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
uint16_t operator/(const uint16_t& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs / static_cast<uint16_t>(rhs);
}

// uint32_t
template<uint16_t INT_BITS, uint16_t FRAC_BITS>
uint32_t operator+(const uint32_t& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs + static_cast<uint32_t>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
uint32_t operator*(const uint32_t& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs * static_cast<uint32_t>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
uint32_t operator-(const uint32_t& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs - static_cast<uint32_t>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
uint32_t operator/(const uint32_t& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs / static_cast<uint32_t>(rhs);
}

// uint64_t
template<uint16_t INT_BITS, uint16_t FRAC_BITS>
uint64_t operator+(const uint64_t& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs + static_cast<uint64_t>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
uint64_t operator*(const uint64_t& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs * static_cast<uint64_t>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
uint64_t operator-(const uint64_t& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs - static_cast<uint64_t>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
uint64_t operator/(const uint64_t& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs / static_cast<uint64_t>(rhs);
}

// float
template<uint16_t INT_BITS, uint16_t FRAC_BITS>
float operator+(const float& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs + static_cast<float>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
float operator*(const float& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs * static_cast<float>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
float operator-(const float& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs - static_cast<float>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
float operator/(const float& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs / static_cast<float>(rhs);
}

// double
template<uint16_t INT_BITS, uint16_t FRAC_BITS>
double operator+(const double& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs + static_cast<double>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
double operator*(const double& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs * static_cast<double>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
double operator-(const double& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs - static_cast<double>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
double operator/(const double& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs / static_cast<double>(rhs);
}

// long double
template<uint16_t INT_BITS, uint16_t FRAC_BITS>
long double operator+(const long double& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs + static_cast<long double>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
long double operator*(const long double& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs * static_cast<long double>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
long double operator-(const long double& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs - static_cast<long double>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
long double operator/(const long double& lhs, const ufixed_point_t<INT_BITS, FRAC_BITS>& rhs) {
	return lhs / static_cast<long double>(rhs);
}

template<uint16_t INT_BITS, uint16_t FRAC_BITS>
ufixed_point_t<INT_BITS, FRAC_BITS> sqrt(const ufixed_point_t<INT_BITS, FRAC_BITS> val) {
	// printf("called sqrt_fixed\n");
	ufixed_point_t<INT_BITS, FRAC_BITS> ret;
	float tmp = val.getValueF();
	// printf("got value tmp %f\n", tmp);
	tmp = sqrtf(tmp);
	// printf("sqrt done: tmp = %f\n", tmp);
	ret = ufixed_point_t<INT_BITS, FRAC_BITS>(static_cast<float>(tmp));
	return ret;
}

#endif /* end of include guard: UFIXED_POINT_OPERATORS_HPP */
