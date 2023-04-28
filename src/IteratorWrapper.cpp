#include "IteratorWrapper.h"

IteratorWrapper::IteratorWrapper(AbstractMatrixIterator * ptr) : _iterator(ptr) {}

void IteratorWrapper::operator++() {

}

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
