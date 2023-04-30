#include "MatrixMemoryRepr.h"

MatrixMemoryRepr::MatrixMemoryRepr(std::size_t rows, std::size_t columns)
    : _dimensions(rows, columns) {}

std::ostream & operator<<(std::ostream & os, const MatrixMemoryRepr & matrix) {
    matrix.print(os);
    return os;
}

size_t MatrixMemoryRepr::rows() const { return _dimensions.rows(); }

size_t MatrixMemoryRepr::columns() const { return _dimensions.columns(); }
