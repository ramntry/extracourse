#include <iomanip>
#include "Complex.h"

Complex Complex::conjugate() const
{
    return Complex(re, -im);
}

Complex Complex::operator -() const
{
    return Complex(-re, -im);
}

Complex Complex::reciproc() const
{
    double sqrNorm = re * re + im * im;
    return Complex(re / sqrNorm, -im / sqrNorm);
}

Complex &Complex::operator +=(Complex const& right)
{
    re += right.re;
    im += right.im;

    return *this;
}

Complex &Complex::operator -=(Complex const& right)
{
    return *this += -right;
}

Complex &Complex::operator *=(Complex const& right)
{
    double t = re * right.re - im * right.im;
    im       = re * right.im + im * right.re;
    re       = t;

    return *this;
}

Complex &Complex::operator /=(Complex const& right)
{
    return *this *= right.reciproc();
}

Complex operator +(Complex left, Complex const& right)
{
    return left += right;
}

Complex operator -(Complex left, Complex const& right)
{
    return left -= right;
}

Complex operator *(Complex left, Complex const& right)
{
    return left *= right;
}

Complex operator /(Complex left, Complex const& right)
{
    return left /= right;
}

std::ostream &operator <<(std::ostream &os, Complex const& z)
{
    return os << '(' << z.re << std::showpos << z.im << "j)" << std::noshowpos;
}
