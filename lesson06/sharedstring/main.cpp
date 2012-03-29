#include <iostream>
#include "SharedString.h"

using namespace std;

int main()
{
    SharedString s("hello");
    SharedString copy = s;

    SharedString s2 = "world";
    s = s2;
    s = s; // is safe
    SharedString s3 = s;
    SharedString s4 = s3; // safe maxRefCounter exceeded - first memcpy()

                          // second memcpy() :(
    char ch = s[0];       // очевидно, что эта строка при выполнении никаким
    cout << ch << endl;   // ... образом не создает возможности изменить
                          // ... разделяемую память. Хочется, чтобы вызывалась здесь
                          // ... константная версия operator []. Как этого добиться?
    cout << copy << ' ' << s << endl;

    SharedString str = "shared string";
    SharedString str2 = str;
    cout << str << " | " << str2 << endl;

    str[6] = '-';
    cout << str << " | " << str2 << endl;  // third memcpy() :)

    str2[6] = '_';
    cout << str << " | " << str2 << endl;

    SharedString sh0 = "something strange";
    SharedString sh1 = "to be";
    SharedString sh12 = sh1;
    SharedString sh2 = " or not ";
    SharedString sh3 = sh0 + sh1 + sh2 + sh12;
    SharedString sh4 = sh3;
    sh4 += " (Shakes";

    SharedString sh5("peare)rubbish rubbish again", 13);
    sh4 += sh5;

    cout << sh4.substr(17, 32) << endl;

    return 0;

    // valgrind OK
}
