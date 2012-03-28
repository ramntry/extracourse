#include "AriphmeticStack.h"


double AriphmeticStack::pop()
{
    double res = s.pop().value;

    currProduct = s.isEmpty() ? 1 : s.top().precalcProduct;
    productBorder.check(s);

    currSumsin = s.isEmpty() ? 0 : s.top().precalcSumsin;
    sumsinBorder.check(s);

    return res;
}

double AriphmeticStack::product()
{
    while (productBorder.hasNext())
    {
        currProduct *= productBorder.getNext().value;
        release || std::cout << "multiply in product()" << std::endl;
        productBorder.next().precalcProduct = currProduct;
    }
    return currProduct;
}

double AriphmeticStack::sumsin()
{
    while (sumsinBorder.hasNext())
    {
        double currValue = sumsinBorder.getNext().value;
        Impl::Iterator it(s);
        while(it.hasNext() && it != sumsinBorder)
        {
            currSumsin += sin(currValue * it.next().value);
            release || std::cout << "sin() in sumsin()" << std::endl;
        }

        sumsinBorder.next().precalcSumsin = currSumsin;
    }
    return currSumsin;
}

std::ostream &operator <<(std::ostream &os, AriphmeticStack &stack)
{
    AriphmeticStack::Impl::Iterator it(stack.s);
    while (it.hasNext())
        os << it.next().value << ' ';

    return os;
}

