#include "BigInt.h"
#include "Rational.h"
#include <iostream>

//#define successShown

bool assertTrue(bool expression, int line) {
	if (!expression) {
		cout << "failure on line #" << line << endl;
	} else {
#ifdef successShown
		cout << "success on line #" << line << endl;
#endif // successShown
	}
	return expression;
}

bool assertFalse(bool expression, int line) {
	if (expression) {
		cout << "failure on line #" << line << endl;
	} else {
#ifdef successShown
		cout << "success on line #" << line << endl;
#endif // successShown
	}
	return !expression;
}

void tests() {
	// Bigints
	{
		BigInt a = 5;
		BigInt b = 5;
		assertTrue(a == b, __LINE__);
	}
	{
		BigInt a = 0;
		BigInt b = 0;
		assertTrue(a == b, __LINE__);
	}
	{
		BigInt a = -5;
		BigInt b = -5;
		assertTrue(a == b, __LINE__);
	}
	{
		BigInt a = 3;
		BigInt b = 3;
		assertFalse(a < b, __LINE__);
	}
	{
		BigInt a = 3;
		BigInt b = 4;
		assertTrue(a < b, __LINE__);
	}
	{
		BigInt a = -3;
		BigInt b = -3;
		assertFalse(a < b, __LINE__);
	}
	{
		BigInt a = -3;
		BigInt b = -4;
		assertFalse(a < b, __LINE__);
	}
	{
		BigInt a = -1;
		BigInt b = 4;
		assertTrue(a < b, __LINE__);
	}
}

int main() {
	tests();
	return 0;
}