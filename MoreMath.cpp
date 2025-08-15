#include "BigInt.h"
#include <iostream>

int main() {
	BigInt a = 0x80000000;
	//a = -a;
	a = a << 1;
	BigInt b = a >> 8;

	cout << "a: " << a.toHex() << "\nb: " << b.toHex();

	return 0;
}
