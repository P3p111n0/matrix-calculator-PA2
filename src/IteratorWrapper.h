#pragma once

#include "AbstractMatrixIterator.h"
#include <memory>

class IteratorWrapper {
  public:
    IteratorWrapper(AbstractMatrixIterator *);
    void operator++();
    MatrixElement operator*() const;
    std::size_t distance(const IteratorWrapper &) const;
    bool operator==(const IteratorWrapper &) const;
    bool operator!=(const IteratorWrapper &) const;

    std::size_t get_matrix_rows() const;
    std::size_t get_matrix_columns() const;

  private:
    std::unique_ptr<AbstractMatrixIterator> _iterator;
};