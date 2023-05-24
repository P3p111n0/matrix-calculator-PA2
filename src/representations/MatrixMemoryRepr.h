#pragma once

#include "../iterators/IteratorWrapper.h"
#include "../matrix_wrapper/MatrixDimensions.h"
#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>

class MatrixMemoryRepr {
  public:
    /**
     * @brief The default constructor is deleted, as there is nothing like
     *        a default matrix representation, let alone a default matrix.
     */
    MatrixMemoryRepr() = delete;

    /**
     * @brief Creates a zero-filled matrix of the specified dimensions
     * @param rows Number of rows of the represented matrix
     * @param columns Number of columns of the represented matrix
     */
    MatrixMemoryRepr(std::size_t rows, std::size_t columns);

    virtual ~MatrixMemoryRepr() = default;

    /**
     * @brief Returns a pointer to a dynamically allocated copy of the matrix, on
     *        which this method was called.
     * @return A raw pointer to the copy. It is up to the programmer to free
     *         this pointer.
     */
    virtual MatrixMemoryRepr * clone() const = 0;

    /**
     * @brief Prints the matrix into the provided output stream in a format using
     *        brackets.
     * @param os Output stream to print to.
     * @param mx Matrix to print.
     * @return Reference to the provided output stream.
     */
    friend std::ostream & operator<<(std::ostream & os, const MatrixMemoryRepr & mx);

    /**
     * @brief Getter returning the amount of rows of the matrix.
     * @return Number of rows of the matrix.
     */
    std::size_t rows() const;

    /**
     * @brief Getter returning the amount of columns of the matrix.
     * @return Number of columns of the matrix.
     */
    std::size_t columns() const;

    /**
     * @brief Return the element in the provided row in the appropriate column.
     *        Standard zero-based indexing is presumed.
     * @param row Row, in which the requested element is located.
     * @param column Column, in which the requested element is located.
     * @return Element in the provided row and column, returns an empty optional
     *         object if the provided indices are out of bounds.
     */
    virtual std::optional<double> at(std::size_t row, std::size_t column) const = 0;

    /**
     * @brief Adds val to the specific row and column using operator+.
     *        Standard zero-based indexing is presumed.
     * @param row Row of the element to add to.
     * @param column Column of the element to add to.
     * @param val Value to add to the element in the specific row and column.
     * @throws std::out_of_range if indices are out of bounds.
     */
    virtual void add(std::size_t row, std::size_t column, double val) = 0;

    /**
     * @brief Replaces the element in the specified row and column with the
     *        provided value. Standard zero-based indexing is presumed.
     * @param row Row of the element to replace.
     * @param column Column of the element to replace.
     * @param val Value to replace the element in the specified row and column
     *            with.
     * @throws std::out_of_range if indices are out of bounds.
     */
    virtual void modify(std::size_t row, std::size_t column, double val) = 0;

    /**
     * @brief Swaps the elements of the 2 specified rows.
     *        Standard zero-based indexing is presumed.
     * @param first_row Index of the first row to be swapped.
     * @param second_row Index of the second row to be swapped.
     * @throws std::out_of_range if indices are out of bounds.
     */
    virtual void swap_rows(std::size_t first_row, std::size_t second_row) = 0;

    /**
     * @brief Checks the efficiency of the representation in the given ratio.
     * @param ratio Ratio of zeroes to the number of elements.
     * @return True is the representation is efficient, false otherwise.
     */
    virtual bool is_efficient(double ratio) const = 0;

    /**
     * @brief Returns a forward iterator to the first element of the matrix.
     * @return A forward iterator to the element of the matrix.
     */
    virtual IteratorWrapper begin() const = 0;

    /**
     * @brief Returns a forward iterator past the last element of the matrix.
     * @return A forward iterator past the last element of the matrix.
     */
    virtual IteratorWrapper end() const = 0;

  protected:
    /**
     * @brief A struct containing the dimensions of the matrix. Gets inherited
     *        by all derived classes.
     */
    MatrixDimensions _dimensions;

    /**
     * @brief A method which prints the matrix into the provided output stream.
     *        Serves as an intermediary for printing using operator<<.
     * @param os An output stream to print the matrix into.
     */
    virtual void print(std::ostream & os) const = 0;
};