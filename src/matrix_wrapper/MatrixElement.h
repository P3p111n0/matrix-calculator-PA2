#pragma once

#include <cstdlib>

/**
 * @brief Struct Position represents the position of a value in a matrix.
 *        It assumes standard zero-based based indexing starting in the
 *        left upper corner of a matrix.
 *
 */
//           0   1   2   .   .   .
//          --- --- ---
//      0  | x   x   x ...
//      1  | x   x   x ...
//      2  | x   x   x
//      .    .   .     .
//      .    .   .       .
struct Position {
    Position() = delete;

    /**
     * @brief Initializes the struct with the provided values.
     * @param row Row of the value referenced by <b>this</b>
     * @param column Column of the value referenced by <b>this</b>
     */
    Position(std::size_t row, std::size_t column) : row(row), column(column) {}
    std::size_t row; // row of the element
    std::size_t column; // column of the element

    /**
     * @brief Compares two Positions by their rows or by their columns, if their
     *        rows are equal.
     * @param rhs The right hand side of the comparison. <b>this</b> < <b>rhs</b>
     * @return True if the row of <b>this</b> is less than row of <b>rhs</b> or
     *         if their rows are equal and column of <b>this</b> is less than
     *         column of <b>rhs</b>, false otherwise.
     */
    inline bool operator<(const Position & rhs) const {
        return row == rhs.row ? column < rhs.column : row < rhs.row;
    };
};

/**
 * @brief MatrixElement represents an element of a matrix and it's position in
 *        the matrix. Primarily used in Iterators, as these values need to be
 *        commonly known.
 */
struct MatrixElement {
    MatrixElement() = delete;

    /**
     * @brief Initializes the struct and it's member Position struct with the
     *        given values.
     * @param r Row of the element, will be used to initialize struct Position.
     * @param col Column of the element, will be used to initialize struct Position.
     * @param val Value at the given position.
     */
    MatrixElement(std::size_t r, std::size_t col, const double & val)
        : position(r, col), value(val) {}

    /**
     * @brief Position of the given value, see struct Position for more
     *        information.
     */
    Position position;

    /**
     * @brief Value at the given position determined by the member
     *        <b>position</b>.
     */
    double value;
};