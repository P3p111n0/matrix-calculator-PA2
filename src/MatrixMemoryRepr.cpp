#include "MatrixMemoryRepr.h"
#include <limits>

MatrixMemoryRepr::MatrixMemoryRepr(std::size_t rows, std::size_t columns)
    : _rows(std::move(rows)), _columns(std::move(columns)) {}

std::ostream & operator<<(std::ostream & os, const MatrixMemoryRepr & matrix) {
    matrix.print(os);
    return os;
}

size_t MatrixMemoryRepr::rows() const { return _rows; }

size_t MatrixMemoryRepr::columns() const { return _columns; }

bool MatrixMemoryRepr::dbl_eq(double lhs, double rhs) {
    constexpr int magic_constant = 420;
    return abs(abs(lhs) - abs(rhs)) <=
           magic_constant * std::numeric_limits<double>::epsilon() *
               (abs(lhs) + abs(rhs));
}
