#pragma once

#include "MatrixMemoryRepr.h"
#include "MatrixElement.h"
#include "Rational.h"
#include <functional>
#include <initializer_list>
#include <map>
#include <vector>

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
     std::map<Position, Rational> _data;
};