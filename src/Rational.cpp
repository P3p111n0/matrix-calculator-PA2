#include "Rational.h"
#include <numeric>

void Rational::simplify() {
    if ((_num < 0 && _denom < 0) || (_num >= 0 && _denom < 0)) {
        _num *= -1;
        _denom *= -1;
    }

    int gcd = std::gcd(_num, _denom);
    _num /= gcd;
    _denom /= gcd;
}

Rational::Rational() : _num(0), _denom(1) {}

Rational::Rational(int numerator) : _num(numerator), _denom(1) {}

Rational::Rational(int numerator, int denominator)
    : _num(numerator), _denom(denominator) {

    if (!_denom) {
        throw std::logic_error("Denominator cannot be equal to zero.");
    }

    simplify();
}
Rational Rational::operator+(const Rational & other) const {
    return {_num * other._denom + other._num * _denom,
                    _denom * other._denom};
}

Rational Rational::operator-(const Rational & other) const {
    return {_num * other._denom - other._num * _denom,
                    _denom * other._denom};
}

Rational Rational::operator*(const Rational & other) const {
    return {_num * other._num, _denom * other._denom};
}

Rational Rational::operator/(const Rational & other) const {
    return {_num * other._denom, _denom * other._num};
}

Rational & Rational::operator+=(const Rational & other) {
    Rational temp = *this + other;
    _num = temp._num;
    _denom = temp._denom;
    return *this;
}

Rational & Rational::operator-=(const Rational & other) {
    Rational temp = *this - other;
    _num = temp._num;
    _denom = temp._denom;
    return *this;
}

Rational & Rational::operator*=(const Rational & other) {
    Rational temp = *this * other;
    _num = temp._num;
    _denom = temp._denom;
    return *this;
}

Rational & Rational::operator/=(const Rational & other) {
    Rational temp = *this / other;
    _num = temp._num;
    _denom = temp._denom;
    return *this;
}

bool Rational::operator<(const Rational & other) const {
    int lcm = std::lcm(_denom, other._denom);
    int lhs_numerator = _num * (lcm / _denom);
    int rhs_numerator = other._num * (lcm / other._denom);

    return lhs_numerator < rhs_numerator;
}

bool Rational::operator==(const Rational & other) const {
    return _num == other._num && _denom == other._denom;
}

bool Rational::operator!=(const Rational & other) const {
    return !(*this == other);
}

std::ostream & operator<<(std::ostream & os, const Rational & x) {
    os << x._num;
    if (x._denom != 1) {
        os << '/' << x._denom;
    }
    return os;
}
double Rational::evaluate() const {
    return _num / static_cast<double>(_denom);
}
