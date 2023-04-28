#include "IteratorWrapper.h"

IteratorWrapper::IteratorWrapper(const AbstractMatrixIterator *) {}

void IteratorWrapper::operator++() {}

MatrixElement IteratorWrapper::operator*() const { return MatrixElement(); }

std::size_t IteratorWrapper::operator-(const IteratorWrapper &) const {
    return 0;
}

bool IteratorWrapper::operator==(const IteratorWrapper &) const {
    return false;
}

bool IteratorWrapper::operator!=(const IteratorWrapper &) const {
    return false;
}
