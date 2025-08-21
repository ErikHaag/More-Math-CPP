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

	Rational abs();
	Rational ceiling();
	Rational floor();
	static Rational gcd(Rational lhs, Rational rhs);
	static Rational lcm(Rational lhs, Rational rhs);
	Rational truncate();

	Rational& operator+=(Rational rhs);
	Rational& operator-=(Rational rhs);
	Rational& operator*=(Rational rhs);
	Rational& operator/=(Rational rhs);
	Rational& operator%=(Rational rhs);

	bool isInfinite();
	bool isInteger();
	bool isNegative();
	bool isZero();
	friend bool operator==(Rational lhs, Rational rhs);
	friend bool operator!=(Rational lhs, Rational rhs);
	friend bool operator<(Rational lhs, Rational rhs);
	friend bool operator>(Rational lhs, Rational rhs);
	friend bool operator<=(Rational lhs, Rational rhs);
	friend bool operator>=(Rational lhs, Rational rhs);
};

