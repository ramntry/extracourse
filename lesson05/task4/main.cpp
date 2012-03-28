#include <iostream>
#include "AriphmeticStack.h"

using namespace std;

double const sqrtPi = sqrt(acos(-1));

void printFuncs(AriphmeticStack const& stack)
{
    cout << "Product: " << stack.product() << '\n'
         << "Sumsin: "  << stack.sumsin()  << endl;

    stack.top();  // OK
//    stack.pop();  // compilation error
}

int main()
{
    AriphmeticStack stack;
    stack.push(5);
    stack.push(10);

    cout << stack.product() << endl;
    cout << stack.top() << endl;
    cout << stack.product() << endl;

    stack.push(2);
    cout << stack.product() << endl;

    stack.pop();
    cout << stack.product() << endl;

    stack.push(3);
    cout << stack.product() << endl;

    AriphmeticStack stack2;
    cout << stack2.product() << endl;

    cout << "-----------------" << endl;

    AriphmeticStack stack3;
    cout << stack3.sumsin() << endl;

    stack3.push(sqrtPi / 2);
    cout << stack3.sumsin() << endl;

    stack3.push(sqrtPi / 3);
    cout << stack3.sumsin() << endl;

    cout << "-----------------" << endl;

    AriphmeticStack stack4;

    stack4.push(2);
    stack4.push(3);
    stack4.push(4);
    stack4.push(5);
    stack4.push(6);

    cout << stack4.product() << endl;

    stack4.pop();
    stack4.pop();
    stack4.pop();
    stack4.pop();

    stack4.push(3);
    stack4.push(2);
    stack4.push(3);

    stack4.pop();
    stack4.pop();
    stack4.pop();
    stack4.pop();

    cout << stack4.size() << ' ' << stack4.capacity() << endl;
    stack4.truncate();
    cout << stack4.size() << ' ' << stack4.capacity() << endl;

    stack4.push(2);
    stack4.push(3);
    stack4.push(2);
    stack4.push(3);

    cout << stack4.product() << endl;
    cout << stack4 << endl;

    cout << "-----------------" << endl;

    stack3.push(sqrtPi);
    cout << stack3.sumsin() << endl;
    cout << stack3.sumsin() << endl;

    printFuncs(stack3);

    stack3.pop();
    stack3.pop();
//    stack3.pop();  // -> StackUnderflowException

    return 0;
}
