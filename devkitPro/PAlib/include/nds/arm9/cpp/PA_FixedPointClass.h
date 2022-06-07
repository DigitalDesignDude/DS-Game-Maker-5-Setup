#pragma once

//////////////////////////////
// Fixed point number class //
//  - fincs                 //
//////////////////////////////

#include <PA9.h>

// Simple fixed point macros
#define PA_FIXED_PRECISION 12
#define PA_FIXED_ONE (1 << PA_FIXED_PRECISION)
#define PA_TOFIXED(a) ((a) << PA_FIXED_PRECISION)
#define PA_TOINTEGER(a) ((a) >> PA_FIXED_PRECISION)

// Internal declaration macros
// NOTE: POD stands for Plain Old Data.

#define _PA_FIXED_POD_OP(op) \
	inline Fixed operator op(int a) const{ return *this op Fixed(a); } \
	inline Fixed operator op(float a) const{ return *this op Fixed(a); }

#define _PA_LOGICAL_POD_OP(op) \
	inline bool operator op(int a) const{ return *this op Fixed(a); } \
	inline bool operator op(float a) const{ return *this op Fixed(a); }

#define _PA_FIXED_ASSIGN_POD_OP(op) \
	inline Fixed& operator op(int a){ *this op Fixed(a); return *this; } \
	inline Fixed& operator op(float a){ *this op Fixed(a); return *this; }

#define _PA_INV_OP(op) \
	inline int operator op(int a, const Fixed& b){ return a op int(b); } \
	inline float operator op(float a, const Fixed& b){ return a op float(b); }

#define _PA_INV_MUL_OP(op) \
	inline int operator op(int a, const Fixed& b){ return int(Fixed(a) op b); } \
	inline float operator op(float a, const Fixed& b){ return a op float(b); }

#define _PA_INV_MOD_OP \
	inline int operator %(int a, const Fixed& b){ return int(Fixed(a) % b); }

#define _PA_INV_ASSIGN_OP(op) \
	inline int& operator op(int& a, const Fixed& b){ return a op int(b); } \
	inline float& operator op(float& a, const Fixed& b){ return a op float(b); }

#define _PA_INV_MULASSIGN_OP(op1,op2) \
	inline int& operator op1(int& a, const Fixed& b){ return a = int(Fixed(a) op2 b); } \
	inline float& operator op1(float& a, const Fixed& b){ return a op1 float(b); }

#define _PA_INV_MODASSIGN_OP \
	inline int& operator %=(int& a, const Fixed& b){ return a = int(Fixed(a) % b); }

#define _PA_INV_LOGICAL_OP(op) \
	inline bool operator op(int a, const Fixed& b){ return a op int(b); } \
	inline bool operator op(float a, const Fixed& b){ return a op float(b); }

// Multiplication/division/modulo/square root macros

#ifdef __thumb__
// When compiling for THUMB:
//  Use the supporting code for fixed point multiplications
#define PA_MULF32(a, b) PA_mulf32(a, b)
//  Use the supporting code for fixed point divisions
#define PA_DIVF32(a, b) PA_divf32(a, b)
//  Use the supporting code for integer divisions
#define PA_DIV32(a, b) div32(a, b)
//  Use the supporting code for fixed point modulo
#define PA_MODF32(a, b) PA_modf32(a, b)
//  Use the supporting code for fixed point square roots
#define PA_SQRTF32(a) PA_sqrtf32(a)
#else
// When compiling for ARM:
//  Use an inline function that does the fixed point multiplication
static inline int PA_MULF32(int a, int b){
	long long result = (a * b) >> PA_FIXED_PRECISION;
	return (int) result;
}
//  Use the supporting code for fixed point divisions
#define PA_DIVF32(a, b) PA_divf32(a, b)
//  Use the supporting code for integer divisions
#define PA_DIV32(a, b) div32(a, b)
//  Use the supporting code for fixed point modulo
#define PA_MODF32(a, b) PA_modf32(a, b)
//  Use the supporting code for fixed point square roots
#define PA_SQRTF32(a) PA_sqrtf32(a)
#endif

/*! \file PA_FixedPointClass.h
    \brief Contains a fixed point wrapper class.
*/

namespace PA{
	/// Fixed-point wrapper class.
	class Fixed{
		int value;

		// Private constructors
		inline Fixed(int dummy, int a){ value = a; }

	public:
		/// Empty constructor.
		inline Fixed() : value(0) { }
		/// Copy constructor.
		inline Fixed(const Fixed& a){ value = a.value; }
		/// int constructor.
		inline Fixed(int a){ value = PA_TOFIXED(a); }
		/// float constructor.
		inline Fixed(float a){ value = int(a * PA_FIXED_ONE); }
		
		/// int cast.
		inline operator int() const{ return PA_TOINTEGER(value); }
		/// float cast.
		inline operator float() const{ return float(value) / PA_FIXED_ONE; }
		/// bool cast.
		inline operator bool() const{ return value != 0; }
		/// char cast.
		inline operator char() const{ return char(int(*this)); }
		/// short cast.
		inline operator short() const{ return short(int(*this)); }
		/// long long cast.
		inline operator long long() const{ return (long long)(int(*this)); }

		/// Assignment operator.
		inline Fixed& operator =(const Fixed& a){ value = a.value; return *this; } _PA_FIXED_ASSIGN_POD_OP(=);

		// Arithmetic operators
		/// Addition operator. int and float versions also available.
		inline Fixed operator +(const Fixed& a) const{ return Fixed(0, value + a.value); } _PA_FIXED_POD_OP(+);
		/// Subtraction operator. int and float versions also available.
		inline Fixed operator -(const Fixed& a) const{ return Fixed(0, value - a.value); } _PA_FIXED_POD_OP(-);
		/// Multiplication operator. int and float versions also available.
		inline Fixed operator *(const Fixed& a) const{ return Fixed(0, PA_MULF32(value, a.value)); }
		/// Division operator. int and float versions also available.
		inline Fixed operator /(const Fixed& a) const{ return Fixed(0, PA_DIVF32(value, a.value)); }
		/// Modulo operator. int and float versions also available.
		inline Fixed operator %(const Fixed& a) const{ return Fixed(0, PA_MODF32(value, a.value)); }

		// Special POD versions of the multiplication and division operators
		inline Fixed operator *(int a) const{ return Fixed(0, value * a); }
		inline Fixed operator /(int a) const{ return Fixed(0, PA_DIV32(value, a)); }
		inline Fixed operator %(int a) const{ return Fixed(0, PA_MODF32(value, PA_TOFIXED(a))); }
		inline Fixed operator *(float a) const{ return *this * Fixed(a); }
		inline Fixed operator /(float a) const{ return *this / Fixed(a);}
		inline Fixed operator %(float a) const{ return *this % Fixed(a);}

		// Unary operators
		/// Pre-increment operator.
		inline Fixed operator ++(){ return Fixed(0, value += PA_FIXED_ONE); }
		/// Pre-decrement operator.
		inline Fixed operator --(){ return Fixed(0, value -= PA_FIXED_ONE); }
		/// Post-increment operator.
		inline Fixed operator ++(int){ return Fixed(0, value += PA_FIXED_ONE); }
		/// Post-decrement operator.
		inline Fixed operator --(int){ return Fixed(0, value -= PA_FIXED_ONE); }
		/// Negation operator.
		inline Fixed operator -() const{ return Fixed(0, -value); }
		/// Binary negation operator.
		inline Fixed operator ~() const{ return Fixed(0, ~value); }

		// Arithmetic assignement operators
		/// Addition and assignment operator. int and float versions also available.
		inline Fixed& operator +=(const Fixed& a){ value += a.value; return *this; } _PA_FIXED_ASSIGN_POD_OP(+=);
		/// Subtraction and assignment operator. int and float versions also available.
		inline Fixed& operator -=(const Fixed& a){ value -= a.value; return *this; } _PA_FIXED_ASSIGN_POD_OP(-=);
		/// Multiplication and assignment operator. int and float versions also available.
		inline Fixed& operator *=(const Fixed& a){ value = PA_MULF32(value, a.value); return *this; } _PA_FIXED_ASSIGN_POD_OP(*=);
		/// Division and assignment operator. int and float versions also available.
		inline Fixed& operator /=(const Fixed& a){ value = PA_DIVF32(value, a.value); return *this; } _PA_FIXED_ASSIGN_POD_OP(/=);
		/// Modulo and assignment operator. int and float versions also available.
		inline Fixed& operator %=(const Fixed& a){ value = PA_MODF32(value, a.value); return *this; } _PA_FIXED_ASSIGN_POD_OP(%=);

		// Comparison operators
		/// Equals operator. int and float versions also available.
		inline bool operator ==(const Fixed& a) const{ return value == a.value; } _PA_LOGICAL_POD_OP(==);
		/// Not-equals operator. int and float versions also available.
		inline bool operator !=(const Fixed& a) const{ return value != a.value; } _PA_LOGICAL_POD_OP(!=);
		/// Less-or-equal operator. int and float versions also available.
		inline bool operator <=(const Fixed& a) const{ return value <= a.value; } _PA_LOGICAL_POD_OP(<=);
		/// Greater-or-equal operator. int and float versions also available.
		inline bool operator >=(const Fixed& a) const{ return value >= a.value; } _PA_LOGICAL_POD_OP(>=);
		/// Less-than operator. int and float versions also available.
		inline bool operator  <(const Fixed& a) const{ return value <  a.value; } _PA_LOGICAL_POD_OP(<);
		/// Greater-than operator. int and float versions also available.
		inline bool operator  >(const Fixed& a) const{ return value >  a.value; } _PA_LOGICAL_POD_OP(>);

		// Shifting operators
		/// Left shift operator.
		inline Fixed operator <<(int a) const{ return Fixed(0, value << a); }
		/// Right shift operator.
		inline Fixed operator >>(int a) const{ return Fixed(0, value >> a); }
		/// Left shift and assign operator.
		inline Fixed& operator <<=(int a){ value <<= a; return *this; }
		/// Right shift and assign operator.
		inline Fixed& operator >>=(int a){ value >>= a; return *this; }

		// Binary operators
		/// Binary AND operator, u32 version.
		inline Fixed operator &(u32 a) const{ return Fixed(0, int(u32(value) & PA_TOFIXED(a))); }
		/// Binary OR operator, u32 version.
		inline Fixed operator |(u32 a) const{ return Fixed(0, int(u32(value) | PA_TOFIXED(a))); }
		/// Binary XOR operator, u32 version.
		inline Fixed operator ^(u32 a) const{ return Fixed(0, int(u32(value) ^ PA_TOFIXED(a))); }
		/// Binary AND operator, Fixed version.
		inline Fixed operator &(const Fixed& a) const{ return Fixed(0, int(u32(value) & u32(a.value))); }
		/// Binary OR operator, Fixed version.
		inline Fixed operator |(const Fixed& a) const{ return Fixed(0, int(u32(value) | u32(a.value))); }
		/// Binary XOR operator, Fixed version.
		inline Fixed operator ^(const Fixed& a) const{ return Fixed(0, int(u32(value) ^ u32(a.value))); }

		// Binary assignment operators
		/// Binary AND assignment, u32 version.
		inline Fixed& operator &=(u32 a){ value = int(u32(value) & PA_TOFIXED(a)); return *this; }
		/// Binary OR assignment, u32 version.
		inline Fixed& operator |=(u32 a){ value = int(u32(value) | PA_TOFIXED(a)); return *this; }
		/// Binary XOR assignment, u32 version.
		inline Fixed& operator ^=(u32 a){ value = int(u32(value) ^ PA_TOFIXED(a)); return *this; }
		/// Binary AND assignment, Fixed version.
		inline Fixed& operator &=(const Fixed& a){ value = int(u32(value) & u32(a.value)); return *this; }
		/// Binary OR assignment, Fixed version.
		inline Fixed& operator |=(const Fixed& a){ value = int(u32(value) | u32(a.value)); return *this; }
		/// Binary XOR assignment, Fixed version.
		inline Fixed& operator ^=(const Fixed& a){ value = int(u32(value) ^ u32(a.value)); return *this; }

		/// Gets the square root.
		inline Fixed sqrt() const{ return Fixed(0, PA_SQRTF32(value)); }
		/// Gets the absolute value.
		inline Fixed abs() const{ return Fixed(0, (value < 0) ? (-value) : value); }
		/// Gets the raw Q12 fixed point number.
		inline int raw() const{ return value; }
		/// Creates a Fixed object using a raw Q12 fixed point number.
		static inline Fixed r2f(int a){ return Fixed(0, a); }
	};

	// Inverse operators
	_PA_INV_OP(+);
	_PA_INV_OP(-);
	_PA_INV_MUL_OP(*);
	_PA_INV_MUL_OP(/);
	_PA_INV_MOD_OP;
	_PA_INV_ASSIGN_OP(+=);
	_PA_INV_ASSIGN_OP(-=);
	_PA_INV_MULASSIGN_OP(*=,*);
	_PA_INV_MULASSIGN_OP(/=,/);
	_PA_INV_MODASSIGN_OP;
	_PA_INV_LOGICAL_OP(==);
	_PA_INV_LOGICAL_OP(!=);
	_PA_INV_LOGICAL_OP(<=);
	_PA_INV_LOGICAL_OP(>=);
	_PA_INV_LOGICAL_OP(<);
	_PA_INV_LOGICAL_OP(>);

	/// Fixed-point point class
	class Point{
	public:
		/// X value.
		Fixed x;
		/// Y value.
		Fixed y;
		/// Convert the object to a PA_Point structure.
		inline operator PA_Point() const{ PA_Point a = {x, y}; return a; }
	};
};

// Undefine internal macros
#undef _PA_FIXED_POD_OP
#undef _PA_LOGICAL_POD_OP
#undef _PA_FIXED_ASSIGN_POD_OP
#undef _PA_INV_OP
#undef _PA_INV_MUL_OP
#undef _PA_INV_MOD_OP
#undef _PA_INV_ASSIGN_OP
#undef _PA_INV_MULASSIGN_OP
#undef _PA_INV_MODASSIGN_OP
#undef _PA_INV_LOGICAL_OP
