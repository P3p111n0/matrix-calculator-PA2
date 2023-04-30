#include "AbstractMatrixIterator.h"

AbstractMatrixIterator::AbstractMatrixIterator(const MatrixDimensions * ptr,
                                               std::size_t row,
                                               std::size_t column)
    : _ptr(ptr), _row(row), _column(column) {}

bool AbstractMatrixIterator::operator==(
    const AbstractMatrixIterator & other) const {
    return _ptr == other._ptr && _row == other._row && _column == other._column;
}

bool AbstractMatrixIterator::operator!=(
    const AbstractMatrixIterator & other) const {
    return !(*this == other);
}

std::size_t AbstractMatrixIterator::get_matrix_rows() const {
    return _ptr->rows();
}

std::size_t AbstractMatrixIterator::get_matrix_columns() const {
    return _ptr->columns();
}
