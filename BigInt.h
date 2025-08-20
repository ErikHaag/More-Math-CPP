#pragma once

#include <list>
#include <string>

using namespace std;

class BigInt {
private:
	list<unsigned long> bits = {};
	bool isNeg = false;
	void condense();

public:
	void setBits(list<unsigned long> b);
	list<unsigned long> getBits();

	void setIsNeg(bool n);
	bool getIsNeg();

	int toInt();
	unsigned int toUint();
	long toLong();
	unsigned long toUlong();

	string toHex() const;
	string toString(int base = 10) const;

	BigInt();
	BigInt(int integer);
	BigInt(unsigned int integer);
	BigInt(long integer);
	BigInt(unsigned long integer);
	BigInt(long long integer);
	BigInt(unsigned long long integer);

	BigInt operator~ ();
	friend BigInt operator&(BigInt lhs, BigInt rhs);
	friend BigInt operator|(BigInt lhs, BigInt rhs);
	friend BigInt operator^(BigInt lhs, BigInt rhs);
	friend BigInt operator<<(BigInt lhs, BigInt rhs);
	friend BigInt operator>>(BigInt lhs, BigInt rhs);

	BigInt& operator++();
	BigInt operator++(int);
	BigInt& operator--();
	BigInt operator--(int);


	BigInt operator-();
	friend BigInt operator+(BigInt lhs, BigInt rhs);
	friend BigInt operator-(BigInt lhs, BigInt rhs);
	friend BigInt operator*(BigInt lhs, BigInt rhs);
	friend BigInt operator/(BigInt lhs, BigInt rhs);
	friend BigInt operator%(BigInt lhs, BigInt rhs);

	BigInt abs();
	static BigInt gcd(BigInt lhs, BigInt rhs);
	static BigInt lcm(BigInt lhs, BigInt rhs);
	BigInt log();
	static BigInt pow(BigInt base, BigInt power);

	BigInt& operator&=(BigInt rhs);
	BigInt& operator|=(BigInt rhs);
	BigInt& operator^=(BigInt rhs);
	BigInt& operator<<=(BigInt rhs);
	BigInt& operator>>=(BigInt rhs);
	BigInt& operator+=(BigInt rhs);
	BigInt& operator-=(BigInt rhs);
	BigInt& operator*=(BigInt rhs);
	BigInt& operator/=(BigInt rhs);
	BigInt& operator%=(BigInt rhs);

	friend bool operator==(BigInt lhs, BigInt rhs);
	friend bool operator!=(BigInt lhs, BigInt rhs);
	friend bool operator<(BigInt lhs, BigInt rhs);
	friend bool operator>(BigInt lhs, BigInt rhs);
	friend bool operator<=(BigInt lhs, BigInt rhs);
	friend bool operator>=(BigInt lhs, BigInt rhs);

};