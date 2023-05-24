#include "SparseMatrixIterator.h"

SparseMatrixIterator::SparseMatrixIterator(
    const SparseMatrix * ptr,
    const SparseMatrixIterator::MapIterator & map_iterator)
    : AbstractMatrixIterator(
          &ptr->_dimensions,
          map_iterator == ptr->_data.end() ? ptr->_dimensions.rows()
                                           : map_iterator->first.row,
          map_iterator == ptr->_data.end() ? 0 : map_iterator->first.column),
      _it(map_iterator), _end(ptr->_data.end()) {}

SparseMatrixIterator::SparseMatrixIterator(
    const MatrixDimensions * ptr, const SparseMatrixIterator::MapIterator & it,
    const SparseMatrixIterator::MapIterator & end_it)
    : AbstractMatrixIterator(ptr, it == end_it ? ptr->rows() : it->first.row,
                             it == end_it ? 0 : it->first.column),
      _it(it), _end(end_it) {}

void SparseMatrixIterator::operator++() {
    ++_it;
    if (_it == _end) {
        _row = _ptr->rows();
        _column = 0;
        return;
    }
    const auto & pos = _it->first;
    _row = pos.row;
    _column = pos.column;
}

MatrixElement SparseMatrixIterator::operator*() const {
    auto & [pos, value] = *_it;
    return {pos.row, pos.column, value};
}

std::size_t
SparseMatrixIterator::distance(const AbstractMatrixIterator & other) const {
    SparseMatrixIterator it_copy(*this);
    std::size_t result = 0;
    while (it_copy != other && _row < _ptr->rows()) {
        ++it_copy;
        ++result;
    }
    return result;
}
