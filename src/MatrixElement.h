#include <cstdlib>

#ifndef MELCRJOS_MATRIXELEMENT_H
#define MELCRJOS_MATRIXELEMENT_H

class MatrixElement {
  public:
    MatrixElement() = delete;
    MatrixElement(std::size_t r, std::size_t col, int val)
        : row(r), column(col), value(val) {}
    std::size_t row;
    std::size_t column;
    int value;
};

#endif // MELCRJOS_MATRIXELEMENT_H
