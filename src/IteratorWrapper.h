#pragma once

#include "AbstractMatrixIterator.h"
#include <memory>

class IteratorWrapper {
  public:
    IteratorWrapper(AbstractMatrixIterator *);
    void operator++();
    MatrixElement operator*() const;
    std::size_t operator-(const IteratorWrapper &) const;
    bool operator==(const IteratorWrapper &) const;
    bool operator!=(const IteratorWrapper &) const;
  private:
    std::unique_ptr<AbstractMatrixIterator> _iterator;
};