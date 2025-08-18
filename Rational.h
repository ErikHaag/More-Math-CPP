#pragma once
#include "BigInt.h"
class Rational {
private:
	BigInt numerator = 0ul;
	BigInt denominator = 1ul;
	void reduce();
public:
	Rational();
	Rational(BigInt numerator, BigInt denominator = BigInt(0ul));

	string toFraction(int base = 10) const;
	string toDecimal(int base = 10) const;


};

