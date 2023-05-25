#pragma once

#include "../iterators/IteratorWrapper.h"
#include "../representations/MatrixMemoryRepr.h"
#include "MatrixFactory.h"
#include <functional>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

/**
 * @brief Matrix is a wrapper around MatrixMemoryRepr, which implements
 *        automatic transitions between representations and implements
 * algorithms common for matrices.
 */
class Matrix {
  public:
    /**
     * @brief Creates a zero-filled matrix of the given dimensions.
     * @param rows Number of desired rows.
     * @param columns Number of desired columns.
     * @param factory Factory used to create the representation.
     * @throws std::invalid_argument if factory.get_initial_repr(rows, columns)
     *                               throws.
     */
    Matrix(std::size_t rows, std::size_t columns, MatrixFactory factory);

    /**
     * @brief Initializes the matrix with the provided initializer list,
     *        dimensions of the matrix are automatically detected.
     * @param init Initializer list used to create the matrix.
     * @param factory Factory used to create an effective representation.
     * @throws std::invalid_argument if factory.get_initial_repr(init) throws
     */
    Matrix(std::initializer_list<std::initializer_list<double>> init,
           MatrixFactory factory);

    /**
     * @brief Copies the provided representation, stores it and converts it
     *        to a more effective one if necessary.
     * @param representation Representation to store in the wrapper.
     * @param factory Factory used for potential optimisations.
     */
    Matrix(const MatrixMemoryRepr & representation, MatrixFactory factory);

    /**
     * @brief Constructs a matrix with the appropriate representation from the
     *        given range. Dimensions are automatically detected.
     * @param begin Start of the given range.
     * @param end End of the given range.
     * @param factory Factory used for the creation of the appropriate
     *                representation.
     */
    Matrix(IteratorWrapper begin, IteratorWrapper end, MatrixFactory factory);

    /**
     * @brief Wraps a value in a 1x1 sparse matrix.
     * @param value Value to be wrapped.
     */
    Matrix(double value);

    /**
     * @brief Creates a deep copy of the provided matrix.
     * @param src Matrix to be copied.
     */
    Matrix(const Matrix & src);

    /**
     * @brief Constructs a matrix from the source, destroying it in the process.
     * @param src Matrix used for the creation process.
     */
    Matrix(Matrix && src) noexcept;

    /**
     * @brief Creates a deep copy of the provided matrix and assigns it to
     *        <b>this</b>
     * @param src Matrix to be copied.
     * @return <b>*this</b>
     */
    Matrix & operator=(const Matrix & src);

    /**
     * @brief Assigns the provided matrix to <b>this</b> by moving, destroying
     *        the source in the process.
     * @param src Matrix, from the data will be moved.
     * @return <b>*this</b>
     */
    Matrix & operator=(Matrix && src) noexcept;

    /**
     * @brief Implements matrix addition. Elements at the same position are
     *        added, the result is stored in a new matrix.
     * @param rhs Matrix to be added to <b>this</b>.
     * @return The resulting matrix from matrix addition.
     * @throws std::invalid_argument if <b>this</b> and <b>rhs</b> don't have
     *                               the same dimensions.
     */
    Matrix operator+(const Matrix & rhs) const;

    /**
     * @brief Implements matrix subtractions. Elements at the same position are
     *        subtracted, the result is stored in a new matrix.
     * @param rhs Matrix to be subtracted from <b>this</b>.
     * @return The resulting matrix from matrix subtraction.
     * @throws std::invalid_argument if <b>this</b> and <b>rhs</b> don't have
     *                               the same dimensions.
     */
    Matrix operator-(const Matrix & rhs) const;

    /**
     * @brief Implements matrix multiplication. Calls the scalar multiplication
     *        method if <b>this</b> or <b>rhs</b> have 1x1 dimensions.
     * @param rhs Matrix to multiply <b>this</b> from the right.
     * @return Product of the matrix multiplication.
     * @throws std::invalid_argument if matrix multiplication is undefined for
     *                               matrices with the given dimensions, ie.
     *                               if <b>this.columns</b> != <b>rhs.rows</b>
     */
    Matrix operator*(const Matrix & rhs) const;

    /**
     * @brief Implements scalar multiplication. Every element of <b>rhs</b> gets
     *        multiplied by <b>value</b>.
     * @param value Value to multiply the matrix by.
     * @param rhs Matrix to be multiplied by <b>value</b>
     * @return
     */
    friend Matrix operator*(double value, const Matrix & rhs);

    /**
     * @brief Creates an iterator pointing to the first element of the matrix
     *        (element at position 0,0). See <b>struct Position</b> for more
     *        information about matrix indexing.
     * @return Iterator to the first element of the matrix.
     */
    IteratorWrapper begin() const;

    /**
     * @brief Creates an iterator pointing past the last element of the matrix
     *        (element at position rows-1, columns-1). See
     *        <b>struct Position</b> for more information about matrix indexing.
     * @return Iterator past the last element of the matrix.
     */
    IteratorWrapper end() const;

    /**
     * @brief Getter for the number of rows of the matrix.
     * @return Number of rows of the matrix.
     */
    std::size_t rows() const;

    /**
     * @brief Getter for the number of columns of the matrix.
     * @return Number of columns of the matrix.
     */
    std::size_t columns() const;

    /**
     * @brief Creates a transposed matrix from <b>this</b>. Additional memory
     *        for copies of the matrix may be needed.
     * @return <b>this</b> transposed.
     */
    Matrix transpose() const;

    /**
     * @brief Unites two matrices by adding the correct amount of rows to
     *        <b>first</b> and appending <b>second</b> below it. Copies of both
     *        matrices are made in the process.
     * @param first The 'matrix on top'
     * @param second The 'matrix on below <b>first</b>'
     * @return Unification of the matrices as described above, ie. matrix of
     *         dimensions
     *         (<b>first</b>.rows + <b>second</b>.rows,
     * <b>first/second</b>.columns), containing copies of both.
     * @throws std::invalid_argument if <b>first</b> and <b>second</b> don't
     *                               have the same amount of columns.
     */
    static Matrix unite(const Matrix & first, const Matrix & second);

    /**
     * @brief Extracts a 'sub-matrix' from <b>this</b> with dimensions of
     *        (<b>new_rows</b>, <b>new_columns</b>) at the given offset. See
     *        <b>struct Position</b> for more information about matrix indexing.
     * @param new_rows Number of rows of the extracted matrix.
     * @param new_columns Number of columns of the extracted matrix.
     * @param offset_rows Row offset, at which the extraction will begin.
     * @param offset_columns Column offset, at which the extraction will begin.
     * @return The extracted matrix.
     * @throws std::invalid_argument if <b>new_rows + offset_rows</b>
     *                               is greater than the number of rows of
     *                               <b>this</b> or if
     *                               <b>new_columns + offset_columns</b>
     *                               is greater than the number of columns of
     *                               <b>columns</b>
     */
    Matrix cut(std::size_t new_rows, std::size_t new_columns,
               std::size_t offset_rows, std::size_t offset_columns) const;

    /**
     * @brief An overload of cut for numbers represented as 1x1 matrices.
     *        Performs tests on parameters and calls
     *        <b>cut</b>(new_rows, new_columns, offset_rows, offset_columns).
     * @throws std::invalid_argument if at least one the parameters doesn't have
     *                               1x1 dimensions.
     * @throws std::invalid_argument if <b>cut</b>(new_rows, new_columns,
     *                               offset_rows, offset_columns) throws.
     */
    Matrix cut(const Matrix & new_rows_mx, const Matrix & new_columns_mx,
               const Matrix & offset_rows_mx,
               const Matrix & offset_columns_mx) const;

    /**
     * @brief Creates an inverse matrix to <b>this</b>, if it exists.
     * @return An inverse matrix to <b>this</b>
     * @throws std::logic_error if <b>this</b> is not a square matrix.
     * @throws std::runtime_error if an inverse matrix to <b>this</b>
     *                            doesn't exist.
     */
    Matrix inverse() const;

    /**
     * @brief Calculates the determinant of <b>this</b>.
     * @return std::nullopt if <b>this</b> isn't a square matrix, the
     * determinant of <b>this</b> otherwise.
     */
    std::optional<double> det() const;

    /**
     * @brief Calculates the rank of <b>this</b>.
     * @return Rank of the matrix.
     */
    std::size_t rank() const;

    /**
     * @brief Implements Gaussian elimination to row echelon form.
     * @return The matrix eliminated into row echelon form.
     */
    Matrix gem() const;

    /**
     * @brief Prints <b>matrix</b> into the given output stream.
     * @param os Stream to print the matrix into.
     * @param matrix Matrix to print.
     * @return os
     */
    friend std::ostream & operator<<(std::ostream & os, const Matrix & matrix);

  private:
    /**
     * @brief A pointer to a memory representation of the given matrix. Utilises
     *        polymorphism.
     */
    std::unique_ptr<MatrixMemoryRepr> _matrix;

    /**
     * @brief A matrix factory used for retrieving representations in
     *        constructors, as well as converting between different
     *        representations.
     */
    MatrixFactory _factory;

    /**
     * @brief A helper function for Gaussian elimination, matrix inversion
     *        and determinant calculation. Swaps rows as necessary to convert
     *        the matrix to row echelon form.
     * @param capture_fn A function which serves to optimise different use cases
     *                   for row-swapping. Is called everytime 2 rows are swapped
     *                   with the indices of the two swapped rows. Defaults to
     *                   an empty lambda function if not provided.
     */
    void gem_swap_rows(std::function<void(std::size_t row1,
                                          std::size_t row2)> && capture_fn);

    /**
     * @brief A helper function for Gaussian elimination and determinant
     *        calculation. Eliminates rows as necessary to allow conversion
     *        of the matrix to row echelon form.
     * @param capture_fn A function used for optimising memory requirements
     *                   when using this function across different methods.
     *                   This function is called everytime a top row is
     *                   subtracted from a row below it. Row and column are
     *                   indices to the first non-zero element in the top row.
     *                   Defaults to an empty lambda when no function is
     *                   provided.
     */
    void gem_row_elim(
        std::function<void(std::size_t row, std::size_t column)> && capture_fn);

    /**
     * @brief Tries to convert the representation, does nothing no optimisation
     *        can be made. Calls MatrixFactory::convert() method.
     */
    void optimize();
};