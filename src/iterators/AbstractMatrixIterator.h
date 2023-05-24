#pragma once

#include "../matrix_wrapper/MatrixDimensions.h"
#include "../matrix_wrapper/MatrixElement.h"

class AbstractMatrixIterator {
  public:
    AbstractMatrixIterator(const MatrixDimensions *, std::size_t, std::size_t);
    virtual ~AbstractMatrixIterator() = default;
    virtual void operator++() = 0;
    virtual MatrixElement operator*() const = 0;
    bool operator==(const AbstractMatrixIterator &) const;
    bool operator!=(const AbstractMatrixIterator &) const;
    virtual std::size_t distance(const AbstractMatrixIterator &) const = 0;

    std::size_t get_matrix_rows() const;
    std::size_t get_matrix_columns() const;

  protected:
    const MatrixDimensions * _ptr;
    std::size_t _row;
    std::size_t _column;
};