#include "MatrixMemoryRepr.h"

MatrixMemoryRepr::MatrixMemoryRepr(std::size_t rows, std::size_t columns)
    : _dimensions(rows, columns) {}

std::ostream & operator<<(std::ostream & os, const MatrixMemoryRepr & matrix) {
    if (matrix.rows() == 1 && matrix.columns() == 1){
        os << matrix.at(0, 0).value();
    } else {
        matrix.print(os);
    }
    return os;
}

size_t MatrixMemoryRepr::rows() const { return _dimensions.rows(); }

size_t MatrixMemoryRepr::columns() const { return _dimensions.columns(); }