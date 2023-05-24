#include "IteratorWrapper.h"

IteratorWrapper::IteratorWrapper(AbstractMatrixIterator * ptr)
    : _iterator(ptr) {}

void IteratorWrapper::operator++() { ++(*_iterator); }

MatrixElement IteratorWrapper::operator*() const { return **_iterator; }

std::size_t IteratorWrapper::distance(const IteratorWrapper & other) const {
    return _iterator->distance(*other._iterator);
}

bool IteratorWrapper::operator==(const IteratorWrapper & other) const {
    return *_iterator == *other._iterator;
}

bool IteratorWrapper::operator!=(const IteratorWrapper & other) const {
    return !(*this == other);
}

std::size_t IteratorWrapper::get_matrix_rows() const {
    return _iterator->get_matrix_rows();
}

std::size_t IteratorWrapper::get_matrix_columns() const {
    return _iterator->get_matrix_columns();
}
