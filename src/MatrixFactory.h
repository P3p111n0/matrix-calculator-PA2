#pragma once

#include "MatrixMemoryRepr.h"

class MatrixFactory {
  public:
    MatrixMemoryRepr * get_initial_repr(std::size_t, std::size_t) const;
    MatrixMemoryRepr * get_initial_repr(
        std::initializer_list<std::initializer_list<double>>) const;
    MatrixMemoryRepr * convert(MatrixMemoryRepr *) const;
  private:
    static constexpr double ratio = 2/3.0;
};
