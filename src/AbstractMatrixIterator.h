#pragma once

#include "MatrixElement.h"
#include "MatrixMemoryRepr.h"

class MatrixMemoryRepr;

class AbstractMatrixIterator {
  public:
    AbstractMatrixIterator(const MatrixMemoryRepr *, std::size_t, std::size_t);
    virtual void operator++() = 0;
    virtual MatrixElement operator*() const = 0;
    bool operator==(const AbstractMatrixIterator &) const;
    bool operator!=(const AbstractMatrixIterator &) const;
    virtual std::size_t operator-(const AbstractMatrixIterator &) const = 0;
  protected:
    const MatrixMemoryRepr * _ptr;
    std::size_t _row;
    std::size_t _column;
};
