#pragma once

#include <list>
#include <string>

using namespace std;

class BigInt {
private:
	list<unsigned long> bits = { 0 };
	bool isNeg = false;
	void condense();

public:
	void setBits(list<unsigned long> b);
	list<unsigned long> getBits();

	void setIsNeg(bool n);
	bool getIsNeg();

	string toHex() const;
	BigInt(int integer);
	BigInt(unsigned int integer);
	BigInt(long integer);
	BigInt(unsigned long integer);

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

	BigInt& operator&= (BigInt lhs);
	BigInt& operator|= (BigInt lhs);
	BigInt& operator^= (BigInt lhs);
	BigInt& operator<<= (BigInt lhs);
	BigInt& operator>>= (BigInt lhs);
	BigInt& operator+= (BigInt lhs);
	BigInt& operator-= (BigInt lhs);

	friend bool operator>= (BigInt rhs, BigInt lhs);
	friend bool operator<= (BigInt rhs, BigInt lhs);
	friend bool operator> (BigInt rhs, BigInt lhs);
	friend bool operator< (BigInt rhs, BigInt lhs);

};