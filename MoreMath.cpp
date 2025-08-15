#include "BigInt.h"
#include <iostream>

int main() {
	BigInt a = 0x80000000ul;
	a = a + 0x81000000ul;
	BigInt b = a >> 1;

	cout << "a: " << a.toHex() << "\nb: " << b.toHex();

	return 0;
}
