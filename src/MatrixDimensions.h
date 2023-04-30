#pragma once

#include <cstdlib>

struct MatrixDimensions{
    MatrixDimensions() = delete;
    MatrixDimensions(std::size_t, std::size_t);
    std::size_t rows() const;
    std::size_t columns() const;
  private:
    std::size_t _rows;
    std::size_t _columns;
};