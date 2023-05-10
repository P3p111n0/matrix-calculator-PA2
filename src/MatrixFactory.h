#pragma once

#include "MatrixMemoryRepr.h"

class MatrixFactory {
  public:
    MatrixFactory() = delete;
    MatrixFactory(double);
    MatrixMemoryRepr * get_initial_repr(std::size_t, std::size_t) const;
    MatrixMemoryRepr * get_initial_repr(
        std::initializer_list<std::initializer_list<double>>) const;
    MatrixMemoryRepr * convert(MatrixMemoryRepr *) const;
  private:
    double _ratio;
};
