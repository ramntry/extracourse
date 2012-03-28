#include <iostream>
#include "Complex.h"

using namespace std;

int main()
{
    Complex z1(1, -2);
    Complex z2(1, 2);

    cout << z1 * z2 << endl;

    Complex z3(2, -7);
    cout << z3 << ' ' << z3.reciproc() << ' ' << z3 * z3.reciproc() << endl;

    Complex z4 = z1 / z2;
    cout << z4 << ' ' << (z4 * z2) / z1 << endl;
    return 0;
}
