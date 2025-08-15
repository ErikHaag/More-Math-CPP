#include "BigInt.h"
#include <climits>
#include <list>
#include <string>

const unsigned long MAX = ULONG_MAX;
const unsigned long ALL_BUT_MSB = MAX >> 1;
const unsigned long MSB = ~ALL_BUT_MSB;
const unsigned long LSB = 1;
const unsigned long ALL_BUT_LSB = ~LSB;

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

#pragma region Constructors

BigInt::BigInt() {
}

BigInt::BigInt(int integer) {
	isNeg = integer < 0;
	bits = { (unsigned long)integer };
}

BigInt::BigInt(unsigned int integer) {
	isNeg = false;
	bits = { (unsigned long)integer };
}

BigInt::BigInt(long integer) {
	isNeg = integer < 0;
	bits = { (unsigned long)integer };
}

BigInt::BigInt(unsigned long integer) {
	isNeg = false;
	bits = { integer };
}

BigInt::BigInt(long long integer) {
	isNeg = integer < 0;
	unsigned long long temp = integer;

	for (; temp != 0; temp >>= BITS_IN_ULONG) {
		bits.push_back(temp & MAX);
	}
}

BigInt::BigInt(unsigned long long integer) {
	for (; integer != 0; integer >>= BITS_IN_ULONG) {
		bits.push_back(integer & MAX);
	}
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
BigInt operator&(BigInt rhs, BigInt lhs) {
	list<unsigned long>::iterator rhsIt = rhs.bits.begin();
	list<unsigned long>::iterator lhsIt = lhs.bits.begin();

	BigInt temp;

	while (rhsIt != rhs.bits.end() && lhsIt != lhs.bits.end()) {
		temp.bits.push_back(*rhsIt & *lhsIt);
		++rhsIt;
		++lhsIt;
	}
	if (rhs.isNeg && rhsIt == rhs.bits.end()) {
		while (lhsIt != lhs.bits.end()) {
			temp.bits.push_back(*lhsIt);
			++lhsIt;
		}
	} else if (lhs.isNeg && lhsIt == lhs.bits.end()) {
		while (rhsIt != rhs.bits.end()) {
			temp.bits.push_back(*rhsIt);
			++rhsIt;
		}
	}
	temp.isNeg = lhs.isNeg && rhs.isNeg;
	return temp;
}

// bitwise or
BigInt operator|(BigInt rhs, BigInt lhs) {
	list<unsigned long>::iterator rhsIt = rhs.bits.begin();
	list<unsigned long>::iterator lhsIt = lhs.bits.begin();

	BigInt temp;

	while (rhsIt != rhs.bits.end() && lhsIt != lhs.bits.end()) {
		temp.bits.push_back(*rhsIt | *lhsIt);
		++rhsIt;
		++lhsIt;
	}
	if (!rhs.isNeg && rhsIt == rhs.bits.end()) {
		while (lhsIt != lhs.bits.end()) {
			temp.bits.push_back(*lhsIt);
			++lhsIt;
		}
	} else if (!lhs.isNeg && lhsIt == lhs.bits.end()) {
		while (rhsIt != rhs.bits.end()) {
			temp.bits.push_back(*rhsIt);
			++rhsIt;
		}
	}
	temp.isNeg = lhs.isNeg || rhs.isNeg;
	return temp;
}

// bitwise xor
BigInt operator^(BigInt rhs, BigInt lhs) {
	list<unsigned long>::iterator rhsIt = rhs.bits.begin();
	list<unsigned long>::iterator lhsIt = lhs.bits.begin();

	unsigned long rhsC = *rhsIt;
	unsigned long lhsC = *lhsIt;

	BigInt temp;

	int state = 0;
	while (state = (rhsIt == rhs.bits.end() ? 0 : 1) | (lhsIt == lhs.bits.end() ? 0 : 2), state != 0) {
		rhsC = ((state & 1) != 0) ? *(rhsIt++) : rhs.isNeg ? MAX : 0;
		lhsC = ((state & 2) != 0) ? *(lhsIt++) : lhs.isNeg ? MAX : 0;
		temp.bits.push_back(rhsC ^ lhsC);
	}
	temp.isNeg = rhs.isNeg != lhs.isNeg;
	temp.condense();
	return temp;
}

// leftshift
BigInt operator<<(BigInt rhs, BigInt lhs) {
	if (lhs.isNeg) {
		return 0ul;
	}

	for (; lhs >= BITS_IN_ULONG; lhs -= BITS_IN_ULONG) {
		rhs.bits.push_front(0ul);
	}

	for (; lhs > 0; --lhs) {
		bool carryP = false;
		bool carryN = false;
		for (list<unsigned long>::iterator it = rhs.bits.begin(); it != rhs.bits.end(); ++it) {
			carryP = carryN;
			carryN = (*it & MSB) == MSB;
			*it = (*it & ALL_BUT_MSB) << 1ul;
			if (carryP) {
				*it |= 1ul;
			}
		}

		if (carryN != rhs.isNeg) {
			rhs.bits.push_back(rhs.isNeg ? ALL_BUT_MSB : 1ul);
		}
	}
	return rhs;
}

BigInt operator>>(BigInt rhs, BigInt lhs) {
	if (lhs.isNeg) {
		return 0ul;
	}

	for (; lhs >= BITS_IN_ULONG; lhs -= BITS_IN_ULONG) {
		rhs.bits.pop_front();
	}

	for (; lhs > 0; --lhs) {
		bool carryP = false;
		bool carryN = false;
		list<unsigned long>::iterator next;
		for (list<unsigned long>::iterator it = rhs.bits.begin(); it != rhs.bits.end(); ++it) {
			next = it;
			++next;
			*it >>= 1ul;
			if (next == rhs.bits.end()) {
				*it |= rhs.isNeg ? MSB : 0ul;
			} else {
				*it |= (*next & 1ul) == 1ul ? MSB : 0ul;
			}
		}
	}
	rhs.condense();
	return rhs;
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


BigInt& BigInt::operator--() {
	*this = ~*this;
	++*this;
	*this = ~*this;
	return *this;
}

BigInt BigInt::operator--(int dummy) {
	BigInt temp = *this;
	--*this;
	return temp;
}

#pragma endregion

#pragma region Arithmetic operators

// unary negation
BigInt BigInt::operator-() {
	BigInt temp = *this;
	temp = ~temp;
	++temp;
	return temp;
}

// binary addition
BigInt operator+(BigInt rhs, BigInt lhs) {
	list<unsigned long>::iterator rhsIt = rhs.bits.begin();
	list<unsigned long>::iterator lhsIt = lhs.bits.begin();

	unsigned long rhsC = 0;
	unsigned long lhsC = 0;

	BigInt temp;
	bool carryN = false;
	bool carryP = false;
	unsigned long i = 0;

	int state = 0;

	while (state = (rhsIt != rhs.bits.end() ? 1 : 0) | (lhsIt != lhs.bits.end() ? 2 : 0), state != 0) {
		rhsC = ((state & 1) == 1) ? *(rhsIt++) : (rhs.isNeg ? MAX : 0);
		lhsC = ((state & 2) == 2) ? *(lhsIt++) : (lhs.isNeg ? MAX : 0);

		carryP = carryN;
		carryN = false;

		unsigned long sum = 0;

		// Why doesn't the addition operator let me determine if an overflow happened?
		if ((rhsC & MSB) != (lhsC & MSB)) {
			sum = (rhsC & ALL_BUT_MSB) + (lhsC & ALL_BUT_MSB);
			if ((sum & MSB) == MSB) {
				sum &= ALL_BUT_MSB;
				carryN = true;
			} else {
				sum |= MSB;
			}
		} else if ((rhsC & lhsC & MSB) == MSB) {
			sum = (rhsC & ALL_BUT_MSB) + (lhsC & ALL_BUT_MSB);
			carryN = true;
		} else {
			sum = rhsC + lhsC;
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

	state = (carryN ? 1 : 0) | (rhs.isNeg ? 2 : 0) | (lhs.isNeg ? 4 : 0);

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

// binary subtraction
BigInt operator-(BigInt rhs, BigInt lhs) {
	return rhs + -lhs;
}

BigInt operator*(BigInt rhs, BigInt lhs) {
	bool negate = rhs.isNeg != lhs.isNeg;
	if (rhs.isNeg) {
		rhs = -rhs;
	}
	if (lhs.isNeg) {
		lhs = -lhs;
	}

	BigInt temp = 0ul;

	for (BigInt mask = 1ul; mask <= lhs; mask <<= 1ul) {
		if ((mask & lhs) == mask) {
			temp += rhs;
		}
		rhs <<= 1ul;
	}

	if (negate) {
		temp = -temp;
	}

	return temp;
}


BigInt BigInt::Log() {
	if (*this <= 0ul) {
		return 0ul;
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

	return l;
}

#pragma endregion

#pragma region Assignment operators

// bitwise and assignment
BigInt& BigInt::operator&=(BigInt lhs) {
	*this = *this & lhs;
	return *this;
}

// bitwise or assignment
BigInt& BigInt::operator|=(BigInt lhs) {
	*this = *this | lhs;
	return *this;
}

// bitwise xor assignment
BigInt& BigInt::operator^=(BigInt lhs) {
	*this = *this ^ lhs;
	return *this;
}

// left bitshift assignment
BigInt& BigInt::operator<<=(BigInt lhs) {
	*this = *this << lhs;
	return *this;
}

// right bitshift assignment
BigInt& BigInt::operator>>=(BigInt lhs) {
	*this = *this >> lhs;
	return *this;
}

// addition assignment
BigInt& BigInt::operator+=(BigInt lhs) {
	*this = *this + lhs;
	return *this;
}

// subtraction assignment
BigInt& BigInt::operator-=(BigInt lhs) {
	*this = *this - lhs;
	return *this;
}

// multiplication assignment
BigInt& BigInt::operator*=(BigInt lhs) {
	*this = *this * lhs;
	return *this;
}

#pragma endregion

#pragma region Comparisons

// Equality
bool operator==(BigInt rhs, BigInt lhs) {
	// if rhs == lhs, then rhs ^ lhs == 0
	rhs ^= lhs;
	if (rhs.isNeg) {
		return false;
	}
	list<unsigned long>::iterator it = rhs.bits.begin();
	if (*it != 0) {
		return false;
	}
	// condense() makes 0 one element long
	++it;
	return it == rhs.bits.end();
}

// Inequality
bool operator!=(BigInt rhs, BigInt lhs) {
	return !(lhs == rhs);
}

// Less than
bool operator< (BigInt rhs, BigInt lhs) {
	// a non-negative number is never less than a negative number
	if (!rhs.isNeg && lhs.isNeg) {
		return false;
	}
	return (rhs - lhs).isNeg;
}

// Greater than
bool operator>(BigInt rhs, BigInt lhs) {
	return lhs < rhs;
}

// Less than or equal to
bool operator<=(BigInt rhs, BigInt lhs) {
	return !(lhs < rhs);
}

// Greater than or equal to
bool operator>=(BigInt rhs, BigInt lhs) {
	return !(rhs < lhs);
}

#pragma endregion