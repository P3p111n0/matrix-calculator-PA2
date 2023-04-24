#include "MatrixMemoryRepr.h"

MatrixMemoryRepr::MatrixMemoryRepr(std::size_t rows, std::size_t columns)
    : _rows(std::move(rows)), _columns(std::move(columns)) {}

std::ostream & operator<<(ostream & os, const MatrixMemoryRepr & matrix) {
    matrix.print(os);
    return os;
}

size_t MatrixMemoryRepr::rows() const { return _rows; }

size_t MatrixMemoryRepr::columns() const { return _columns; }