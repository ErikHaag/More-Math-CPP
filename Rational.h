#pragma once
#include "BigInt.h"

class Rational {
private:
	BigInt numerator = 0ul;
	BigInt denominator = 1ul;
public:
	void simplify();

	Rational();
	Rational(BigInt n);
	Rational(BigInt n, BigInt d);

	string toFraction(int base = 10);
	string toDecimal(int base = 10);

	Rational operator-();
	friend Rational operator+(Rational lhs, Rational rhs);
	friend Rational operator-(Rational lhs, Rational rhs);
	friend Rational operator*(Rational lhs, Rational rhs);
	friend Rational operator/(Rational lhs, Rational rhs);
	friend Rational operator%(Rational lhs, Rational rhs);

	Rational ceiling();
	Rational floor();
	Rational truncate();

	bool isInfinite();
	bool isInteger();
	bool isNegative();
	bool isZero();

	Rational& operator+=(Rational rhs);
	Rational& operator-=(Rational rhs);
	Rational& operator*=(Rational rhs);
	Rational& operator/=(Rational rhs);
	Rational& operator%=(Rational rhs);

	friend bool operator==(Rational lhs, Rational rhs);
	friend bool operator!=(Rational lhs, Rational rhs);
	friend bool operator<(Rational lhs, Rational rhs);
	friend bool operator>(Rational lhs, Rational rhs);
	friend bool operator<=(Rational lhs, Rational rhs);
	friend bool operator>=(Rational lhs, Rational rhs);
};

