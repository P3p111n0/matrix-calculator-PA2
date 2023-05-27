#include "DenseMatrixIterator.h"

DenseMatrixIterator::DenseMatrixIterator(const DenseMatrix * ptr,
                                         std::size_t row, std::size_t column)
    : AbstractMatrixIterator(&ptr->_dimensions, row, column), _data(ptr->_data) {
    while (_row < get_matrix_rows() && _data[_row][_column] == 0){
        next_element();
    }
}

DenseMatrixIterator::DenseMatrixIterator(
    const MatrixDimensions * ptr, const DenseMatrixIterator::DenseMatrixContainer & vec,
    std::size_t row, std::size_t column) : AbstractMatrixIterator(ptr, row, column), _data(vec) {
    while ( _row < get_matrix_rows() && _data[_row][_column] == 0){
        next_element();
    }
}

void DenseMatrixIterator::operator++() {
    next_element();
    while (_row < get_matrix_rows() && _data[_row][_column] == 0) {
        next_element();
    }
}

MatrixElement DenseMatrixIterator::operator*() const {
    return {_row, _column, _data[_row][_column]};
}

std::size_t
DenseMatrixIterator::distance(const AbstractMatrixIterator & other) const {
    DenseMatrixIterator it_copy(*this);
    if (_row < get_matrix_rows() && _data[_row][_column] == 0){
        ++it_copy;
    }
    std::size_t result = 0;
    while (it_copy != other && _row < get_matrix_rows()) {
        ++it_copy;
        ++result;
    }
    return result;
}

void DenseMatrixIterator::next_element() {
    ++_column;
    if (_column >= get_matrix_columns()) {
        ++_row;
        _column = 0;
    }
}
