#pragma once

#include "MatrixMemoryRepr.h"
#include "Rational.h"
#include <functional>
#include <initializer_list>
#include <map>
#include <vector>

#ifndef MELCRJOS_SPARSEMATRIX_H
#define MELCRJOS_SPARSEMATRIX_H

class SparseMatrix : public MatrixMemoryRepr {
    friend class SparseMatrixIterator;
  public:
    SparseMatrix(size_t, size_t);
    SparseMatrix(std::initializer_list<std::initializer_list<Rational>>);
    MatrixMemoryRepr * clone() const override;
    std::optional<Rational> at(std::size_t, std::size_t) const override;
    void add(std::size_t, std::size_t, const Rational &) override;
    void modify(std::size_t, std::size_t, const Rational &) override;
    void swap_rows(std::size_t, std::size_t) override;

    IteratorWrapper begin() const override;
    IteratorWrapper end() const override;

  protected:
    void print(std::ostream &) const override;

  private:
    struct Position {
        std::size_t row;
        std::size_t col;
        inline bool operator<(const Position &) const;
    };
     std::map<Position, Rational> _data;
};

#endif // MELCRJOS_SPARSEMATRIX_H
