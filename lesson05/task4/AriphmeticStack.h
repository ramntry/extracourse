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

    double const& top() const
    { return s.top().value; }

    bool isEmpty() const
    { return s.isEmpty(); }

    int size() const
    { return s.size(); }

    int capacity() const
    { return s.capacity(); }

    void truncate()
    { s.truncate(); }

    double product() const;
    double sumsin() const;

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

    mutable Impl::Iterator productBorder;
    mutable double currProduct;

    mutable Impl::Iterator sumsinBorder;
    mutable double currSumsin;

friend
    std::ostream &operator <<(std::ostream &os, AriphmeticStack &stack);
};
