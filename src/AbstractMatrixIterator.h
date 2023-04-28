#pragma once

#include "MatrixElement.h"
#include "MatrixMemoryRepr.h"

class AbstractMatrixIterator {
  public:
    virtual void operator++() = 0;
    virtual MatrixElement operator*() const = 0;
    virtual bool operator==(const AbstractMatrixIterator *) const = 0;
    virtual bool operator!=(const AbstractMatrixIterator *) const = 0;
    virtual std::size_t operator-(const AbstractMatrixIterator *) const = 0;
  protected:
    MatrixMemoryRepr * _ptr;
};

