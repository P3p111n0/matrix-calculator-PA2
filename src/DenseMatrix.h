#pragma once

#include <vector>
#include "MatrixMemoryRepr.h"
#include "Rational.h"

#ifndef MATRIXCALCULATOR_DENSEMATRIX_H
#define MATRIXCALCULATOR_DENSEMATRIX_H

class DenseMatrix : public MatrixMemoryRepr {
    friend class DenseMatrixIterator;
  public:
    DenseMatrix(std::size_t, std::size_t);
    DenseMatrix(std::initializer_list<std::initializer_list<Rational>>);

    MatrixMemoryRepr * clone() const override;
    std::optional<Rational> at(std::size_t, std::size_t) const override;
    void add(std::size_t, std::size_t,
             const Rational &) override;
    void modify(std::size_t, std::size_t,
                const Rational &) override;
    void swap_rows(std::size_t, std::size_t) override;

    IteratorWrapper begin() const override;
    IteratorWrapper end() const override;

  protected:
    void print(std::ostream &) const override;
  private:
    std::vector<std::vector<Rational>> _data;
};

#endif // MATRIXCALCULATOR_DENSEMATRIX_H
