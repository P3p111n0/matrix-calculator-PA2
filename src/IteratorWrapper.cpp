#include "IteratorWrapper.h"

IteratorWrapper::IteratorWrapper(AbstractMatrixIterator * ptr) : _iterator(ptr) {}

void IteratorWrapper::operator++() {
    ++(*_iterator);
}

MatrixElement IteratorWrapper::operator*() const {
    return **_iterator;
}

std::size_t IteratorWrapper::operator-(const IteratorWrapper & other) const {
    return *_iterator - *(other._iterator);
}

bool IteratorWrapper::operator==(const IteratorWrapper & other) const {
    return *_iterator == *other._iterator;
}

bool IteratorWrapper::operator!=(const IteratorWrapper & other) const {
    return !(*this == other);
}
