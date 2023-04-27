#include "MatrixMemoryRepr.h"
#include <limits>

MatrixMemoryRepr::MatrixMemoryRepr(std::size_t rows, std::size_t columns)
    : _rows(rows), _columns(columns) {}

std::ostream & operator<<(std::ostream & os, const MatrixMemoryRepr & matrix) {
    matrix.print(os);
    return os;
}

size_t MatrixMemoryRepr::rows() const { return _rows; }

size_t MatrixMemoryRepr::columns() const { return _columns; }
