#include "SparseMatrix.h"

bool SparseMatrix::MatrixElementComparator::operator()(
    const MatrixElement & lhs, const MatrixElement & rhs) const {
    if (lhs.row != rhs.row) {
        return lhs.row < rhs.row;
    }
    return lhs.column < rhs.column;
}

SparseMatrix::SparseMatrix(size_t rows, size_t columns)
    : MatrixMemoryRepr(rows, columns) {}

SparseMatrix::SparseMatrix(const std::vector<MatrixElement> & dump)
    : MatrixMemoryRepr(dump.back().row, dump.back().column) {

    for (const auto & element : dump) {
        if (element.value != 0) {
            _data.emplace(element);
        }
    }
}



