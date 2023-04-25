#include "MatrixMemoryRepr.h"
#include <set>
#include <initializer_list>
#include <vector>

#ifndef MELCRJOS_SPARSEMATRIX_H
#define MELCRJOS_SPARSEMATRIX_H

class SparseMatrix : public MatrixMemoryRepr {
  public:
    SparseMatrix(size_t, size_t);
    SparseMatrix(const std::vector<std::vector<double>> &);
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
    const std::vector<std::vector<double>> dump() const override;

  protected:
    void print(std::ostream &) const override;

  private:
    struct MatrixElement {
        MatrixElement() = delete;
        MatrixElement(std::size_t r, std::size_t col, int val)
                : row(r), column(col), value(val) {}
        std::size_t row;
        std::size_t column;
        double value;
        inline bool operator<(const MatrixElement & rhs) const;
    };
    std::set<MatrixElement> _data;

    double calc_det() const;
    size_t calc_rank() const;
};

#endif // MELCRJOS_SPARSEMATRIX_H
