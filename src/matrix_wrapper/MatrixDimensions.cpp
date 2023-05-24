#include "MatrixDimensions.h"

MatrixDimensions::MatrixDimensions(std::size_t rows, std::size_t columns)
    : _rows(rows), _columns(columns) {}

std::size_t MatrixDimensions::rows() const { return _rows; }

std::size_t MatrixDimensions::columns() const { return _columns; }
