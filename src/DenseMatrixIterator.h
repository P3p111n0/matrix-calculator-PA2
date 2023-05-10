#pragma once

#include "AbstractMatrixIterator.h"
#include <vector>
#include "DenseMatrix.h"


class DenseMatrixIterator : public AbstractMatrixIterator {
    using DenseMatrixContainer = std::vector<std::vector<double>>;

  public:
    DenseMatrixIterator(const DenseMatrix *, std::size_t,
                        std::size_t);
    void operator++() override;
    MatrixElement operator*() const override;
    std::size_t distance(const AbstractMatrixIterator & other) const override;

  private:
    const DenseMatrixContainer & _data;
    std::size_t _matrix_rows;
    std::size_t _matrix_columns;
    void next_element();
};