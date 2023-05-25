#pragma once

#include <cstdlib>

/**
 * @brief Represents dimensions of a matrix, is usually used by matrix
 *        representations.
 */
struct MatrixDimensions{
    MatrixDimensions() = delete;

    /**
     * @brief Initializes the struct with the given values. Assumes mathematical
     *        indexing starting from 1.
     * @param rows Number of rows of the given representation.
     * @param columns Number of columns of the given representation.
     */
    MatrixDimensions(std::size_t rows, std::size_t columns);
    std::size_t rows() const;
    std::size_t columns() const;
  private:
    /**
     * @brief Used to store the amount of rows the given matrix has. This
     *        Is accessed by getters to prevent accidental overwriting, which
     *        could potentially be destructive.
     */
    std::size_t _rows;

    /**
     * @brief Used to store the amount of columns the given matrix has. This
     *        Is accessed by getters to prevent accidental overwriting, which
     *        could potentially be destructive.
     */
    std::size_t _columns;
};