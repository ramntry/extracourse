#pragma once

#include <cmath>
#include "Stack.h"
#include "StretchableStack.h"

class AriphmeticStack : public DynamicStack<double>
{
public:
    AriphmeticStack()
        : productBorder(s)
        , currProduct(1.0)

        , sumsinBorder(s)
        , currSumsin(0.0)
    {}

    void push(double value)
    { s.push(AriphmeticItem(value)); }

    double pop();

    double const& top()
    { return s.top().value; }

    bool isEmpty() const
    { return s.isEmpty(); }

    int size() const
    { return s.size(); }

    int capacity() const
    { return s.capacity(); }

    void truncate()
    { s.truncate(); }

    double product();
    double sumsin();

private:
    struct AriphmeticItem
    {
        AriphmeticItem()
        {}

        AriphmeticItem(double _value)
            : value(_value)
        {}

        double value;

        double precalcProduct;
        double precalcSumsin;
    };

    typedef StretchableStack<AriphmeticItem> Impl;

    Impl s;

    Impl::Iterator productBorder;
    double currProduct;

    Impl::Iterator sumsinBorder;
    double currSumsin;

friend
    std::ostream &operator <<(std::ostream &os, AriphmeticStack &stack);
};

double AriphmeticStack::pop()
{
    double res = s.pop().value;

    currProduct = s.isEmpty() ? 1 : s.top().precalcProduct;
    productBorder.toTop(s);

    currSumsin = s.isEmpty() ? 0 : s.top().precalcSumsin;
    sumsinBorder.toTop(s);

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
