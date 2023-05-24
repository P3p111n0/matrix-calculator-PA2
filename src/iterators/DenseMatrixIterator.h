#pragma once

#include "../matrix_wrapper/MatrixDimensions.h"
#include "../representations/DenseMatrix.h"
#include "AbstractMatrixIterator.h"
#include <vector>

class DenseMatrixIterator : public AbstractMatrixIterator {
    using DenseMatrixContainer = std::vector<std::vector<double>>;

  public:
    DenseMatrixIterator(const DenseMatrix *, std::size_t, std::size_t);
    DenseMatrixIterator(const MatrixDimensions *, const DenseMatrixContainer &,
                        std::size_t, std::size_t);
    void operator++() override;
    MatrixElement operator*() const override;
    std::size_t distance(const AbstractMatrixIterator & other) const override;

  private:
    const DenseMatrixContainer & _data;
    void next_element();
};