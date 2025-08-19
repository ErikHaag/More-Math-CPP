#include "BigInt.h"
#include "Rational.h"
#include <iostream>

int main() {
	Rational a = Rational(1ul, 0ul);
	a.simplify();
	cout << a.toFraction();
	return 0;
}