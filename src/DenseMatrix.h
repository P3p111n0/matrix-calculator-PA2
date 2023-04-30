#pragma once

#include "MatrixMemoryRepr.h"
#include <vector>

class DenseMatrix : public MatrixMemoryRepr {
    friend class DenseMatrixIterator;

  public:
    DenseMatrix(std::size_t, std::size_t);
    DenseMatrix(std::initializer_list<std::initializer_list<double>>);

    MatrixMemoryRepr * clone() const override;
    std::optional<double> at(std::size_t, std::size_t) const override;
    void add(std::size_t, std::size_t, double) override;
    void modify(std::size_t, std::size_t, double) override;
    void swap_rows(std::size_t, std::size_t) override;

    IteratorWrapper begin() const override;
    IteratorWrapper end() const override;

  protected:
    void print(std::ostream &) const override;

  private:
    std::vector<std::vector<double>> _data;
};