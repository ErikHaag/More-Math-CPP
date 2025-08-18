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
	friend BigInt operator& (BigInt rhs, BigInt lhs);
	friend BigInt operator| (BigInt rhs, BigInt lhs);
	friend BigInt operator^ (BigInt rhs, BigInt lhs);
	friend BigInt operator<< (BigInt rhs, BigInt lhs);
	friend BigInt operator>> (BigInt rhs, BigInt lhs);

	BigInt& operator++();
	BigInt operator++(int);
	BigInt& operator--();
	BigInt operator--(int);


	BigInt operator-();
	friend BigInt operator+ (BigInt rhs, BigInt lhs);
	friend BigInt operator- (BigInt rhs, BigInt lhs);
	friend BigInt operator* (BigInt rhs, BigInt lhs);
	friend BigInt operator/ (BigInt rhs, BigInt lhs);
	friend BigInt operator% (BigInt rhs, BigInt lhs);
	BigInt log();

	BigInt& operator&= (BigInt lhs);
	BigInt& operator|= (BigInt lhs);
	BigInt& operator^= (BigInt lhs);
	BigInt& operator<<= (BigInt lhs);
	BigInt& operator>>= (BigInt lhs);
	BigInt& operator+= (BigInt lhs);
	BigInt& operator-= (BigInt lhs);
	BigInt& operator*= (BigInt lhs);
	BigInt& operator/= (BigInt lhs);
	BigInt& operator%= (BigInt lhs);

	friend bool operator== (BigInt rhs, BigInt lhs);
	friend bool operator!= (BigInt rhs, BigInt lhs);
	friend bool operator< (BigInt rhs, BigInt lhs);
	friend bool operator> (BigInt rhs, BigInt lhs);
	friend bool operator<= (BigInt rhs, BigInt lhs);
	friend bool operator>= (BigInt rhs, BigInt lhs);

};