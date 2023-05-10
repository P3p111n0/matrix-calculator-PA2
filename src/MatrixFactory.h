#pragma once

#include "MatrixMemoryRepr.h"
#include <vector>

class MatrixFactory {
  public:
    MatrixFactory() = delete;
    explicit MatrixFactory(double);
    MatrixMemoryRepr * get_initial_repr(std::size_t, std::size_t) const;
    MatrixMemoryRepr * get_initial_repr(
        std::initializer_list<std::initializer_list<double>>) const;
    MatrixMemoryRepr * get_initial_repr(double) const;
    MatrixMemoryRepr * get_initial_repr(const std::vector<std::vector<double>> &) const;
    MatrixMemoryRepr * convert(MatrixMemoryRepr *) const;
  private:
    double _ratio;
};
