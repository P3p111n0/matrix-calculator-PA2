#include "MatrixMemoryRepr.h"
#include "Rational.h"
#include <functional>
#include <initializer_list>
#include <set>
#include <vector>

#ifndef MELCRJOS_SPARSEMATRIX_H
#define MELCRJOS_SPARSEMATRIX_H

class SparseMatrix : public MatrixMemoryRepr {
    using MemoryDump = std::vector<std::vector<Rational>>;

  public:
    SparseMatrix(size_t, size_t);
    SparseMatrix(const MemoryDump &);
    SparseMatrix(std::initializer_list<std::initializer_list<Rational>>);
    MatrixMemoryRepr * clone() const override;
    Rational at(std::size_t, std::size_t) const override;
    void add(std::size_t, std::size_t, const Rational &) override;
    void modify(std::size_t, std::size_t, const Rational &) override;
    void swap_rows(std::size_t, std::size_t) override;
    std::vector<std::vector<Rational>> dump() const override;

  protected:
    void print(std::ostream &) const override;

  private:
    struct MatrixElement {
        MatrixElement() = delete;
        MatrixElement(std::size_t r, std::size_t col, const Rational & val)
            : row(r), column(col), value(val) {}
        std::size_t row;
        std::size_t column;
        Rational value;
        inline bool operator<(const MatrixElement & rhs) const;
    };
    std::set<MatrixElement> _data;
};

#endif // MELCRJOS_SPARSEMATRIX_H
