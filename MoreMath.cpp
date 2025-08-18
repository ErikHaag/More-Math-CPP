#include "BigInt.h"
#include <iostream>

int main() {
	BigInt a = -5l;
	BigInt b = 3ul;
	cout << BigInt::pow(a, b).toString();
	return 0;
}
