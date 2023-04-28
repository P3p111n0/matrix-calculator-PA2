#pragma once

#include <iostream>

class Rational {
public:
    Rational();
    Rational(int);
    Rational(int, int);
    Rational operator+ (const Rational &) const;
    Rational operator- (const Rational &) const;
    Rational operator* (const Rational &) const;
    Rational operator/ (const Rational &) const;
    Rational & operator+=(const Rational &);
    Rational & operator-=(const Rational &);
    Rational & operator*=(const Rational &);
    Rational & operator/=(const Rational &);

    bool operator<(const Rational &) const;
    bool operator==(const Rational &) const;
    bool operator!=(const Rational &) const;
    friend std::ostream & operator<<(std::ostream &, const Rational &);
    double evaluate() const;
private:
    void simplify();

    int _num;
    int _denom;
};