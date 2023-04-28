#pragma once

#include "Rational.h"

struct MatrixElement {
    MatrixElement() = delete;
    MatrixElement(std::size_t r, std::size_t col, const Rational & val)
        : row(r), column(col), value(val) {}
    std::size_t row;
    std::size_t column;
    Rational value;
};