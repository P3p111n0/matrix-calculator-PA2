#include "MatrixMemoryRepr.h"
#include <set>
#include <initializer_list>
#include <vector>

#ifndef MELCRJOS_SPARSEMATRIX_H
#define MELCRJOS_SPARSEMATRIX_H

class SparseMatrix : public MatrixMemoryRepr {
  public:
    SparseMatrix(size_t, size_t);
    SparseMatrix(const std::vector<MatrixElement> &);
    SparseMatrix(std::initializer_list<std::initializer_list<int>>);

    MatrixMemoryRepr * clone() const override;

    double det() override;
    double det() const override;
    std::size_t rank() override;
    std::size_t rank() const override;
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

    double calc_det() const;
    size_t calc_rank() const;
};

#endif // MELCRJOS_SPARSEMATRIX_H
