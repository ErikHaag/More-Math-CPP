#include "Rational.h"
#include <stdexcept>

const range_error INDETERMINATE = range_error("Indeterminate form!");

void Rational::simplify() {
	BigInt f = BigInt::gcd(numerator, denominator);
	if (f == 0) {
		throw INDETERMINATE;
	}
	numerator /= f;
	denominator /= f;
	if (denominator.getIsNeg()) {
		numerator = -numerator;
		denominator = -denominator;
	}
}

#pragma region Constructors

Rational::Rational() {
	simplify();
}

Rational::Rational(BigInt n) {
	numerator = n;
}

Rational::Rational(BigInt n, BigInt d) {
	numerator = n;
	denominator = d;
	simplify();
}

#pragma endregion

#pragma region Display

string Rational::toFraction(int base) {
	if (denominator == 0) {
		return (numerator.getIsNeg() ? "Negative infinity" : "Infinity");
	}
	if (denominator == 1) {
		return numerator.toString(base);
	}
	return numerator.toString(base) + "/" + denominator.toString(base);
}

#pragma endregion

#pragma region Arithmetic operators

Rational Rational::operator-() {
	Rational temp = *this;
	temp.numerator = -temp.numerator;
	return temp;
}

Rational operator+(Rational lhs, Rational rhs) {
	Rational temp;
	switch ((lhs.denominator == 0 ? 1 : 0) | (rhs.denominator == 0 ? 2 : 0)) {
		case 1:
			return lhs;
		case 2:
			return rhs;
		case 3:
			if (lhs.numerator.getIsNeg() != rhs.numerator.getIsNeg()) {
				throw INDETERMINATE;
			}
			return lhs;
		case 0:
		default:
			break;
	}
	temp.numerator = lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator;
	temp.denominator = lhs.denominator * rhs.denominator;
	temp.simplify();
	return temp;
}

Rational operator-(Rational lhs, Rational rhs) {
	return lhs + -rhs;
}

Rational operator*(Rational lhs, Rational rhs) {
	Rational temp;
	temp.numerator = lhs.numerator * rhs.numerator;
	temp.denominator = lhs.denominator * rhs.denominator;
	temp.simplify();
	return temp;
}

#pragma endregion
