#pragma once

#include <cstdlib>

struct Position {
    Position() = delete;
    Position(std::size_t row, std::size_t column) : row(row), column(column) {}
    std::size_t row;
    std::size_t column;
    inline bool operator<(const Position & rhs) const {
        return row == rhs.row ? column < rhs.column : row < rhs.row;;
    };
};

struct MatrixElement {
    MatrixElement() = delete;
    MatrixElement(std::size_t r, std::size_t col, const double & val)
        : position(r, col), value(val) {}
    Position position;
    double value;
};