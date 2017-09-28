# fixedpoint
Fixed Point C++ support designed for x86 and x86_64 architectures.
The goal is to enable approximate computing by applying reduced precision computation.

`fixed_point_t<I,F>` can be used as data type to represent a number with
 - `I` bits for the integer part (including sign)
 - `F` bits for the fractional part

`I` and `F` must be greater than zero

`I + F` must be a valid size for a standard integer data type
`{ 8, 16, 32, 64, 128}`
