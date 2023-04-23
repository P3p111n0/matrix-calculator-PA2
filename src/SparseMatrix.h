#include "MatrixMemoryRepr.h"
#include <set>

#ifndef MELCRJOS_SPARSEMATRIX_H
#define MELCRJOS_SPARSEMATRIX_H

class SparseMatrix : public MatrixMemoryRepr {
  public:
    SparseMatrix(size_t, size_t);
    SparseMatrix(const std::vector<MatrixElement> &);
    MatrixMemoryRepr * clone() const override;
    int det() override;
    int det() const override;
    int rank() override;
    int rank() const override;
    void gem() override;
    void inverse() override;
    void transpose() override;
    void unite(const MatrixMemoryRepr &) override;
    std::vector<MatrixElement> dump() const override;

  protected:
    void print(std::ostream &) const override;

  private:
    struct MatrixElementComparator {
        inline bool operator()(const MatrixElement &, const MatrixElement &) const;
    };
    std::set<MatrixElement, MatrixElementComparator> _data;

    int calc_det() const;
    int calc_rank() const;
};

#endif // MELCRJOS_SPARSEMATRIX_H
