#pragma once

#ifndef MATRIXCALCULATOR_RATIONAL_H
#define MATRIXCALCULATOR_RATIONAL_H

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
private:
    void simplify();

    int _num;
    int _denom;
};


#endif //MATRIXCALCULATOR_RATIONAL_H
