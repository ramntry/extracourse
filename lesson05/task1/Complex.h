#pragma once

#include <iostream>

class Complex
{
public:
    Complex(double real = 0.0, double imaginary = 0.0)
        : re(real)
        , im(imaginary)
    {}

    Complex conjugate() const;
    Complex operator -() const;
    Complex reciproc() const;

    Complex &operator +=(Complex const& right);
    Complex &operator -=(Complex const& right);

    Complex &operator *=(Complex const& right);
    Complex &operator /=(Complex const& right);

private:
    double re;
    double im;

friend Complex operator +(Complex left, Complex const& right);
friend Complex operator -(Complex left, Complex const& right);
friend Complex operator *(Complex left, Complex const& right);
friend Complex operator /(Complex left, Complex const& right);

friend std::ostream &operator <<(std::ostream &os, Complex const& z);
};
