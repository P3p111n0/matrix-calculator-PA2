#pragma once

#include "../representations/SparseMatrix.h"
#include "AbstractMatrixIterator.h"
#include <map>

class SparseMatrixIterator : public AbstractMatrixIterator {
    using MapIterator = std::map<Position, double>::const_iterator;

  public:
    SparseMatrixIterator(const SparseMatrix *,
                         const MapIterator &);
    SparseMatrixIterator(const MatrixDimensions *, const MapIterator &, const MapIterator &);
    void operator++() override;
    MatrixElement operator*() const override;
    std::size_t distance(const AbstractMatrixIterator & other) const override;

  private:
    MapIterator _it;
    MapIterator _end;
};
