#include "BigInt.h"
#include <iostream>

int main() {
	BigInt a = -10;
	BigInt b = -3;
	BigInt c = a % b;
	cout << c.toHex();
	return 0;
}
