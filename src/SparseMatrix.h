#include "MatrixMemoryRepr.h"
#include <set>

#ifndef MELCRJOS_SPARSEMATRIX_H
#define MELCRJOS_SPARSEMATRIX_H

class SparseMatrix : public MatrixMemoryRepr {
  public:
    SparseMatrix(size_t, size_t);
    SparseMatrix(const std::vector<MatrixElement> &);
  private:
    struct MatrixElementComparator {
        inline bool operator()(const MatrixElement &, const MatrixElement &) const;
    };
    std::set<MatrixElement, MatrixElementComparator> _data;
};

#endif // MELCRJOS_SPARSEMATRIX_H
