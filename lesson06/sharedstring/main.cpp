#include <iostream>
#include <string>
#include "SharedString.h"


using namespace std;

void cpy(SharedString *dst, SharedString const* src, int n)
{
    for (int i = 0; i < n; ++i)
        dst[i] = src[i];
}

void print(SharedString const* a, int n)
{
    for (int i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < a[i].size(); ++j)
            cout.put(a[i][j]);  // There are no deep copy
        cout.put(' ');
    }
    cout.put('\n');

//    a[0][0] = '!';  // compilation error (that's OK)
}

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
/*
    SharedString sh0 = "something strange ";
    SharedString sh1 = "to be";
    SharedString sh12 = sh1;
    SharedString sh2 = " or not ";
    SharedString sh3 = sh0 + sh1 + sh2 + sh12;
    SharedString sh4 = sh3;
    sh4 += " (Shakes";

    SharedString sh5("peare) rubbish rubbish again", 14);
    sh4 += sh5;
    sh4.local_cstr()[0] = 'S';  // compilation error. That's OK
    cout << sh4.local_cstr() << endl;

    char *deepCopy = sh4.dynamic_cstr();
    deepCopy[0] = 'S';
    cout << deepCopy << endl;
    delete[] deepCopy;

    cout << sh4.substr(18, 33) << endl;

*/
    SharedString nums[] = { "one", "two", "three", "four", "five" };
    SharedString dst[5]; // There are no heap alloc
    cpy(dst, nums, 5);   // There are no real copy
    print(dst, 5);
/*
    SharedString small = "tr";
    small += "mr"; // в данной реализации += в этой строке выполняется просто
                   // прорва операций. Этот оператор нужно писать независимо,
                   // в идеале - в отдельных перегрузках для const char *-
                   // и SharedString-аргументов. Но лениво :) Кстати говоря,
                   // даже при толковой его реализации operator + все одно
                   // нужно оставлять независимым - через += его также эффективно
                   // реализовать не представляется возможным.

    string stdStr(small.local_cstr());  // fast and safe solution
    cout << stdStr;

    SharedString empty = "";
    char *tmp = empty.dynamic_cstr();
    cout << tmp << endl;
    delete[] tmp;

    string sh4cp = sh4;
    cout << sh4cp << endl;

    SharedString *heap = new SharedString(sh4cp);
    delete heap;
*/

    return 0;

    // valgrind OK
}
