#include "Rational.h"
#include <stdexcept>

const range_error INDETERMINATE = range_error("Indeterminate form!");

void Rational::simplify() {
	if (denominator.getIsNeg()) {
		numerator = -numerator;
		denominator = -denominator;
	}
	BigInt f = BigInt::gcd(numerator, denominator);
	if (f == 0) {
		throw INDETERMINATE;
	}
	numerator /= f;
	denominator /= f;
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
	switch ((lhs.isInfinite() ? 1 : 0) | (rhs.isInfinite() ? 2 : 0)) {
		case 1:
			return lhs;
		case 2:
			return rhs;
		case 3:
			if (lhs.isNegative() != rhs.isNegative()) {
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

Rational operator/(Rational lhs, Rational rhs) {
	Rational temp;
	temp.numerator = lhs.numerator * rhs.denominator;
	temp.denominator = lhs.denominator * rhs.numerator;
	temp.simplify();
	return temp;
}

Rational operator%(Rational lhs, Rational rhs) {
	if (lhs.isZero() || rhs.isZero()) {
		return Rational(0ul);
	}
	if (rhs.isInfinite()) {
		return rhs.isNegative() ? -lhs : lhs;
	}
	Rational flooredDiv = (lhs / rhs).floor();
	lhs -= flooredDiv * rhs;
	return lhs;
}

Rational Rational::ceiling() {
	if (this->isInfinite()) {
		return *this;
	}
	BigInt r = this->numerator % -this->denominator;
	return Rational((this->numerator - r) / this->denominator);
}

Rational Rational::floor() {
	if (this->isInfinite()) {
		return *this;
	}
	return Rational(this->numerator / this->denominator);
}

Rational Rational::truncate() {
	if (this->isNegative()) {
		return this->ceiling();
	}
	return this->floor();
}

#pragma endregion

#pragma region Assignment operators

Rational& Rational::operator+=(Rational lhs) {
	*this = *this + lhs;
}

#pragma endregion

#pragma region Comparisons


bool Rational::isInfinite() {
	return this->denominator == 0;
}

bool Rational::isInteger() {
	return this->denominator == 1;
}

bool Rational::isNegative() {
	return this->numerator.getIsNeg();
}

bool Rational::isZero() {
	return this->numerator == 0;
}

#pragma endregion