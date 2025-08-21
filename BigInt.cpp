// todo: make custom list type to save memory
#include "BigInt.h"
#include <list>
#include <string>
#include <stdexcept>

const unsigned long MAX = ~0ul;
const unsigned long ALL_BUT_MSB = MAX >> 1;
const unsigned long MSB = ~ALL_BUT_MSB;
const unsigned long LSB = 1;
const unsigned long ALL_BUT_LSB = ~LSB;

const unsigned long BYTES_IN_ULLONG = (unsigned long)sizeof(unsigned long long);
const unsigned long BYTES_IN_ULONG = (unsigned long)sizeof(unsigned long);
const unsigned long NYBBLES_IN_ULONG = BYTES_IN_ULONG << 1ul;
const unsigned long BITS_IN_ULONG = BYTES_IN_ULONG << 3ul;

using namespace std;

void BigInt::condense() {
	unsigned long test = isNeg ? MAX : 0ul;
	size_t size = bits.size();
	while (!bits.empty() && bits.back() == test) {
		bits.pop_back();
		size--;
	}
}

#pragma region Getters and setters

void BigInt::setBits(list<unsigned long> b) {
	bits = b;
}

list<unsigned long> BigInt::getBits() {
	return bits;
}

void BigInt::setIsNeg(bool n) {
	isNeg = n;
}

bool BigInt::getIsNeg() {
	return isNeg;
}

#pragma endregion

#pragma region Conversions

int BigInt::toInt() {
	if (bits.empty()) {
		return isNeg ? -1 : 0;
	}
	const int maxInt = ~0u >> 1u;
	return (bits.front() & maxInt) | (isNeg ? ~maxInt : 0);
}

unsigned int BigInt::toUint() {
	if (bits.empty()) {
		return isNeg ? -1 : 0;
	}
	return bits.front();
}

long BigInt::toLong() {
	if (bits.empty()) {
		return isNeg ? -1 : 0;
	}
	return (bits.front() & ALL_BUT_MSB) | (isNeg ? MSB : 0);
}

unsigned long BigInt::toUlong() {
	if (bits.empty()) {
		return isNeg ? -1 : 0;
	}
	return bits.front();
}

#pragma endregion

string BigInt::toHex() const {
	const char digits[17] = "0123456789ABCDEF";
	BigInt temp = *this;
	bool negative = false;
	if (temp.isNeg) {
		negative = true;
		temp = -temp;
	}
	string s = "";
	for (list<unsigned long>::iterator i = temp.bits.begin(); i != temp.bits.end(); ++i) {
		for (unsigned long j = 0; j < NYBBLES_IN_ULONG; ++j) {
			s = digits[*i >> 4 * j & 0x0f] + s;
		}
	}
	if (s == "") {
		s = "0";
	}
	return (negative ? "-0x" : "0x") + s;
}

string BigInt::toString(int base) const {
	if (!(base >= 2 && base <= 36)) {
		throw range_error("Base is outside the range [2,36]");
	}
	if (*this == 0) {
		return "0";
	}
	BigInt bBase = base;
	const char digits[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	BigInt temp = *this;
	bool negative = false;
	if (temp.isNeg) {
		negative = true;
		temp = -temp;
	}
	string s = "";
	while (temp > 0ul) {
		unsigned int remainder = (temp % bBase).toUint();
		s = digits[remainder] + s;
		temp /= bBase;
	}
	if (negative) {
		s = "-" + s;
	}
	return s;
}

#pragma region Constructors

BigInt::BigInt() {
}

BigInt::BigInt(int integer) {
	isNeg = integer < 0;
	bits = { (unsigned long)integer };
	this->condense();
}

BigInt::BigInt(unsigned int integer) {
	isNeg = false;
	bits = { (unsigned long)integer };
	this->condense();
}

BigInt::BigInt(long integer) {
	isNeg = integer < 0;
	bits = { (unsigned long)integer };
	this->condense();
}

BigInt::BigInt(unsigned long integer) {
	isNeg = false;
	bits = { integer };
	this->condense();
}

BigInt::BigInt(long long integer) {
	isNeg = integer < 0;
	unsigned long long temp = integer;

	for (; temp != 0; temp >>= BITS_IN_ULONG) {
		bits.push_back(temp & MAX);
	}
	this->condense();
}

BigInt::BigInt(unsigned long long integer) {
	for (; integer != 0; integer >>= BITS_IN_ULONG) {
		bits.push_back(integer & MAX);
	}
	this->condense();
}

#pragma endregion

#pragma region Bitwise operations

// bitwise not
BigInt BigInt::operator~() {
	BigInt temp = *this;
	for (list<unsigned long>::iterator it = temp.bits.begin(); it != temp.bits.end(); ++it) {
		*it = ~*it;
	}
	temp.isNeg = !temp.isNeg;
	return temp;
}
// bitwise and
BigInt operator&(BigInt lhs, BigInt rhs) {
	list<unsigned long>::iterator lhsIt = lhs.bits.begin();
	list<unsigned long>::iterator rhsIt = rhs.bits.begin();

	BigInt temp;

	while (lhsIt != lhs.bits.end() && rhsIt != rhs.bits.end()) {
		temp.bits.push_back(*lhsIt & *rhsIt);
		++lhsIt;
		++rhsIt;
	}
	if (lhs.isNeg && lhsIt == lhs.bits.end()) {
		while (rhsIt != rhs.bits.end()) {
			temp.bits.push_back(*rhsIt);
			++rhsIt;
		}
	} else if (rhs.isNeg && rhsIt == rhs.bits.end()) {
		while (lhsIt != lhs.bits.end()) {
			temp.bits.push_back(*lhsIt);
			++lhsIt;
		}
	}
	temp.isNeg = rhs.isNeg && lhs.isNeg;
	return temp;
}

// bitwise or
BigInt operator|(BigInt lhs, BigInt rhs) {
	list<unsigned long>::iterator lhsIt = lhs.bits.begin();
	list<unsigned long>::iterator rhsIt = rhs.bits.begin();

	BigInt temp;

	while (lhsIt != lhs.bits.end() && rhsIt != rhs.bits.end()) {
		temp.bits.push_back(*lhsIt | *rhsIt);
		++lhsIt;
		++rhsIt;
	}
	if (!lhs.isNeg && lhsIt == lhs.bits.end()) {
		while (rhsIt != rhs.bits.end()) {
			temp.bits.push_back(*rhsIt);
			++rhsIt;
		}
	} else if (!rhs.isNeg && rhsIt == rhs.bits.end()) {
		while (lhsIt != lhs.bits.end()) {
			temp.bits.push_back(*lhsIt);
			++lhsIt;
		}
	}
	temp.isNeg = rhs.isNeg || lhs.isNeg;
	return temp;
}

// bitwise xor
BigInt operator^(BigInt lhs, BigInt rhs) {
	list<unsigned long>::iterator lhsIt = lhs.bits.begin();
	list<unsigned long>::iterator rhsIt = rhs.bits.begin();

	unsigned long lhsC = 0ul;
	unsigned long rhsC = 0ul;

	BigInt temp;

	int state = 0;
	while (state = (lhsIt == lhs.bits.end() ? 0 : 1) | (rhsIt == rhs.bits.end() ? 0 : 2), state != 0) {
		lhsC = ((state & 1) != 0) ? *(lhsIt++) : lhs.isNeg ? MAX : 0;
		rhsC = ((state & 2) != 0) ? *(rhsIt++) : rhs.isNeg ? MAX : 0;
		temp.bits.push_back(lhsC ^ rhsC);
	}
	temp.isNeg = lhs.isNeg != rhs.isNeg;
	temp.condense();
	return temp;
}

// bitshift left
BigInt operator<<(BigInt lhs, BigInt rhs) {
	if (rhs.isNeg) {
		return 0ul;
	}

	for (; rhs >= BITS_IN_ULONG; rhs -= BITS_IN_ULONG) {
		lhs.bits.push_front(0ul);
	}

	for (; rhs > 0; --rhs) {
		bool carryP = false;
		bool carryN = false;
		for (list<unsigned long>::iterator it = lhs.bits.begin(); it != lhs.bits.end(); ++it) {
			carryP = carryN;
			carryN = (*it & MSB) == MSB;
			*it = (*it & ALL_BUT_MSB) << 1ul;
			if (carryP) {
				*it |= 1ul;
			}
		}

		if (carryN != lhs.isNeg) {
			lhs.bits.push_back(lhs.isNeg ? ALL_BUT_MSB : 1ul);
		}
	}
	return lhs;
}

// bitshift right
BigInt operator>>(BigInt lhs, BigInt rhs) {
	if (rhs.isNeg) {
		return 0ul;
	}

	for (; rhs >= BITS_IN_ULONG; rhs -= BITS_IN_ULONG) {
		lhs.bits.pop_front();
	}

	for (; rhs > 0; --rhs) {
		bool carryP = false;
		bool carryN = false;
		list<unsigned long>::iterator next;
		for (list<unsigned long>::iterator it = lhs.bits.begin(); it != lhs.bits.end(); ++it) {
			next = it;
			++next;
			*it >>= 1ul;
			if (next == lhs.bits.end()) {
				*it |= lhs.isNeg ? MSB : 0ul;
			} else {
				*it |= (*next & 1ul) == 1ul ? MSB : 0ul;
			}
		}
	}
	lhs.condense();
	return lhs;
}

#pragma endregion

#pragma region Increment and Decrement

// prefix increment
BigInt& BigInt::operator++() {
	list<unsigned long>::iterator it = bits.begin();
	// -1 case
	if (isNeg && bits.empty()) {
		isNeg = false;
		return *this;
	}

	bool carry = true;
	while (it != bits.end()) {
		if (*it != MAX) {
			++(*it);
			carry = false;
			break;
		}
		*it = 0ul;
		++it;
	}
	if (carry) {
		bits.push_back(1ul);
	}
	if (isNeg) {
		this->condense();
	}
	return *this;
}

// postfix increment
BigInt BigInt::operator++(int dummy) {
	BigInt temp = *this;
	++*this;
	return temp;
}

// prefix decrement
BigInt& BigInt::operator--() {
	*this = ~*this;
	++*this;
	*this = ~*this;
	return *this;
}

// postfix decrement
BigInt BigInt::operator--(int dummy) {
	BigInt temp = *this;
	--*this;
	return temp;
}

#pragma endregion

#pragma region Arithmetic operators

// negation
BigInt BigInt::operator-() {
	BigInt temp = *this;
	temp = ~temp;
	++temp;
	return temp;
}

// addition
BigInt operator+(BigInt lhs, BigInt rhs) {
	list<unsigned long>::iterator lhsIt = lhs.bits.begin();
	list<unsigned long>::iterator rhsIt = rhs.bits.begin();

	unsigned long lhsC = 0;
	unsigned long rhsC = 0;

	BigInt temp;
	bool carryN = false;
	bool carryP = false;
	unsigned long i = 0;

	int state = 0;

	while (state = (lhsIt != lhs.bits.end() ? 1 : 0) | (rhsIt != rhs.bits.end() ? 2 : 0), state != 0) {
		lhsC = ((state & 1) == 1) ? *(lhsIt++) : (lhs.isNeg ? MAX : 0);
		rhsC = ((state & 2) == 2) ? *(rhsIt++) : (rhs.isNeg ? MAX : 0);

		carryP = carryN;
		carryN = false;

		unsigned long sum = 0;

		// Why doesn't the addition operator let me determine if an overflow happened?
		if ((lhsC & MSB) != (rhsC & MSB)) {
			sum = (lhsC & ALL_BUT_MSB) + (rhsC & ALL_BUT_MSB);
			if ((sum & MSB) == MSB) {
				sum &= ALL_BUT_MSB;
				carryN = true;
			} else {
				sum |= MSB;
			}
		} else if ((lhsC & rhsC & MSB) == MSB) {
			sum = (lhsC & ALL_BUT_MSB) + (rhsC & ALL_BUT_MSB);
			carryN = true;
		} else {
			sum = lhsC + rhsC;
		}

		if (carryP) {
			if (sum == MAX) {
				sum = 0ul;
				carryN = true;
			} else {
				++sum;
			}
		}
		temp.bits.push_back(sum);
	}

	state = (carryN ? 1 : 0) | (lhs.isNeg ? 2 : 0) | (rhs.isNeg ? 4 : 0);

	switch (state) {
		case 1:
			temp.bits.push_back(1ul);
			break;
		case 2:
		case 4:
		case 7:
			temp.isNeg = true;
			break;
		case 3:
		case 5:
			break;
		case 6:
			temp.bits.push_back(ALL_BUT_LSB);
			temp.isNeg = true;
			break;
		default:
			break;
	}
	temp.condense();
	return temp;
}

// subtraction
BigInt operator-(BigInt lhs, BigInt rhs) {
	return lhs + -rhs;
}

// multiplication
BigInt operator*(BigInt lhs, BigInt rhs) {
	bool negate = lhs.isNeg != rhs.isNeg;
	if (lhs.isNeg) {
		lhs = -lhs;
	}
	if (rhs.isNeg) {
		rhs = -rhs;
	}

	// TODO: maybe implement karatsuba multiplication?

	BigInt temp = 0ul;

	for (BigInt mask = 1ul; mask <= rhs; mask <<= 1ul) {
		if ((mask & rhs) == mask) {
			temp += lhs;
		}
		lhs <<= 1ul;
	}

	if (negate) {
		return -temp;
	}
	return temp;
}

// division
BigInt operator/(BigInt lhs, BigInt rhs) {
	if (rhs == 0) {
		throw exception("Division by zero!");
	}
	bool negate = lhs.isNeg != rhs.isNeg;
	if (lhs.isNeg) {
		lhs = -lhs;
	}
	if (rhs.isNeg) {
		rhs = -rhs;
	}

	BigInt quotient;
	BigInt i = lhs.log() - rhs.log();
	BigInt subend = rhs << i;
	BigInt mask = 1ul << i;
	while (i >= 0 && lhs >= rhs) {
		if (lhs >= subend) {
			lhs -= subend;
			quotient |= mask;
		}
		mask >>= 1ul;
		subend >>= 1ul;
		--i;
	}
	if (negate) {
		return -quotient;
	}
	return quotient;
}

// modulus
BigInt operator%(BigInt lhs, BigInt rhs) {
	bool lhsNeg = lhs.isNeg;
	bool rhsNeg = rhs.isNeg;
	if (lhs.isNeg) {
		lhs = -lhs;
	}
	if (rhs.isNeg) {
		rhs = -rhs;
	}
	if (lhs == 0ul || rhs <= 1ul) {
		return 0ul;
	}
	BigInt i = lhs.log() - rhs.log();
	BigInt subend = rhs << i;
	while (i >= 0ul && lhs >= rhs) {
		if (lhs >= subend) {
			lhs -= subend;
		}
		subend >>= 1ul;
		--i;
	}
	if (lhs != 0) {
		if (lhsNeg) {
			// Can we please make the modulus operator always return a positive number when the divisor is positive,
			// I've never wanted (-1) % 5 == -1
			lhs = rhs - lhs;
		}
		if (rhsNeg) {
			lhs -= rhs;
		}
	}
	return lhs;
}

#pragma endregion

#pragma region Other operations

// absolute value
BigInt BigInt::abs() {
	return this->isNeg ? -*this : *this;
}

// greatest common divisor
BigInt BigInt::gcd(BigInt lhs, BigInt rhs) {
	BigInt L = lhs.abs();
	BigInt G = rhs.abs();
	BigInt temp = L;
	if (L > G) {
		L = G;
		G = temp;
	}
	while (L != 0) {
		temp = L;
		L = G % L;
		G = temp;
	}
	return G;
}

// least common multiple
BigInt BigInt::lcm(BigInt lhs, BigInt rhs) {
	return lhs * rhs / BigInt::gcd(lhs, rhs);
}

// floored base 2 logarithm
BigInt BigInt::log() {
	if (*this <= 0ul) {
		return -1l;
	}
	BigInt l = 0;
	list<unsigned long>::iterator it = this->bits.begin();
	++it;
	for (; it != this->bits.end(); ++it) {
		l += BITS_IN_ULONG;
	}
	--it;
	unsigned long mask = 1ul;
	for (unsigned long i = 0; i < BITS_IN_ULONG && mask <= *it; ++i, mask <<= 1) {
		++l;
	}
	--l;
	return l;
}

// exponentiation
BigInt BigInt::pow(BigInt base, BigInt pow) {
	if (pow == 0) {
		// empty product
		return 1ul;
	}
	if (pow.isNeg || base == 0ul) {
		return 0ul;
	}
	BigInt temp = base;
	BigInt product = 1ul;
	BigInt mask = 1ul;
	while (mask <= pow) {
		if ((pow & mask) == mask) {
			product *= temp;
		}
		mask <<= 1ul;
		temp *= temp;
	}
	return product;
}

#pragma endregion

#pragma region Assignment operators

// bitwise and assignment
BigInt& BigInt::operator&=(BigInt rhs) {
	*this = *this & rhs;
	return *this;
}

// bitwise or assignment
BigInt& BigInt::operator|=(BigInt rhs) {
	*this = *this | rhs;
	return *this;
}

// bitwise xor assignment
BigInt& BigInt::operator^=(BigInt rhs) {
	*this = *this ^ rhs;
	return *this;
}

// left bitshift assignment
BigInt& BigInt::operator<<=(BigInt rhs) {
	*this = *this << rhs;
	return *this;
}

// right bitshift assignment
BigInt& BigInt::operator>>=(BigInt rhs) {
	*this = *this >> rhs;
	return *this;
}

// addition assignment
BigInt& BigInt::operator+=(BigInt rhs) {
	*this = *this + rhs;
	return *this;
}

// subtraction assignment
BigInt& BigInt::operator-=(BigInt rhs) {
	*this = *this - rhs;
	return *this;
}

// multiplication assignment
BigInt& BigInt::operator*=(BigInt rhs) {
	*this = *this * rhs;
	return *this;
}

// division assignment
BigInt& BigInt::operator/=(BigInt rhs) {
	*this = *this / rhs;
	return *this;
}

// modulus assignment
BigInt& BigInt::operator%=(BigInt rhs) {
	*this = *this % rhs;
	return *this;
}

#pragma endregion

#pragma region Comparisons

// Equality
bool operator==(BigInt lhs, BigInt rhs) {
	// if lhs == rhs, then lhs ^ rhs == 0
	lhs ^= rhs;
	if (lhs.isNeg) {
		return false;
	}
	list<unsigned long>::iterator it = lhs.bits.begin();
	// condense() makes "0" zero elements long
	return it == lhs.bits.end();
}

// Inequality
bool operator!=(BigInt lhs, BigInt rhs) {
	return !(rhs == lhs);
}

// Less than
bool operator< (BigInt lhs, BigInt rhs) {
	// a non-negative number is never less than a negative number
	if (lhs.isNeg != rhs.isNeg) {
		return lhs.isNeg;
	}
	return (lhs - rhs).isNeg;
}

// Greater than
bool operator>(BigInt lhs, BigInt rhs) {
	return rhs < lhs;
}

// Less than or equal to
bool operator<=(BigInt lhs, BigInt rhs) {
	return !(rhs < lhs);
}

// Greater than or equal to
bool operator>=(BigInt lhs, BigInt rhs) {
	return !(lhs < rhs);
}

#pragma endregion