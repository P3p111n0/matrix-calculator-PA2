#pragma once

#include "MatrixMemoryRepr.h"
#include <vector>

/**
 * @brief Despite being called DenseMatrix, this class serves solely as a
 *        container for values and as such can't guarantee any memory efficiency.
 *        If you want to use a memory efficient implementation of a matrix,
 *        please use the Matrix class.
 */
class DenseMatrix : public MatrixMemoryRepr {
    friend class DenseMatrixIterator;

  public:

    /**
     * @brief Created a zero-filled matrix of the given dimensions. This
     *        representation is inefficient for this type of matrix, as no
     *        memory optimizations are performed.
     * @param rows Number of rows of the resulting matrix.
     * @param columns Number of columns of the resulting matrix.
     * @throws std::invalid_argument if rows or columns are equal to zero.
     */
    DenseMatrix(std::size_t rows, std::size_t columns);

    /**
     * @brief Creates a matrix from an initializer list with the appropriate
     *        dimensions. No checks are ran on the inputted values. Passing a
     *        sparse matrix this way will result in an inefficient representation.
     * @param initializerList Initializer list to construct the matrix from.
     * @throws std::invalid_argument if the number of columns is not consistent
     *                               across all rows, or if the number of rows
     *                               or columns is equal to zero.
     */
    DenseMatrix(std::initializer_list<std::initializer_list<double>> initializerList);

    /**
     * @brief Creates a matrix with the values from the range given by the begin
     *        and end iterators. No checks are performed on the range, so if
     *        a range representing a sparse matrix is given on the input,
     *        the resulting representation may not efficient.
     * @param begin An iterator to the first element of the range.
     * @param end An iterator past the last element of the given range.
     */
    DenseMatrix(IteratorWrapper begin, IteratorWrapper end);

    /**
     * @brief Returns a pointer to a dynamically allocated copy of the matrix.
     *        It is the programmer's responsibility to free this pointer.
     * @return A pointer to a dynamically allocated copy of the matrix.
     */
    MatrixMemoryRepr * clone() const override;

    /**
     * @brief Returns the element at the given indices, or an empty optional
     *        object, if the indices exceed the dimensions of the matrix.
     *        Standard zero-based indexing is presumed.
     * @param row Row of the element in question.
     * @param column Column of the element in question.
     * @return Element at the given indices, or an empty optional object, if
     *         the indices exceed the dimensions of the matrix.
     */
    std::optional<double> at(std::size_t row, std::size_t column) const override;

    /**
     * @brief Increments the element at the given indices by the provided value
     *        using operator+. Using this method might result in the
     *        representation of the matrix becoming inefficient. Standard
     *        zero-based indexing is presumed.
     * @param row Row of the element to add to.
     * @param column Column of the element to add to.
     * @param value Value to add to the element at the given indices.
     * @throws std::out_of_range if the indices exceed the dimensions of the
     *                           matrix.
     */
    void add(std::size_t row, std::size_t column, double value) override;

    /**
     * @brief Changes the element's value at the given indices to the provided
     *        value. Using this method might result in the representation
     *        becoming inefficient. Standard zero-based indexing is presumed.
     * @param row Row of the element, which will get changed.
     * @param column Column of the element, which will get changed.
     * @param value Value, which will replace the value at given indices.
     * @throws std::out_of_range if the indices exceed the dimensions of the
     *                           matrix.
     */
    void modify(std::size_t row, std::size_t column, double value) override;

    /**
     * @brief Swaps the elements of the row at the first_row index with the
     *        elements at the second_row index. Standard zero-based indexing
     *        is presumed.
     * @param first_row Index of the row to be swapped with the row at second_row.
     * @param second_row Index of the row to be swapped with the row at first_row.
     * @throws std::out_of_range if at least one of the indices exceeds the
     *                           number of rows of the matrix.
     */
    void swap_rows(std::size_t first_row, std::size_t second_row) override;

    /**
     * @brief Determines, whether the representation is effective in the
     *        given ratio.
     * @param ratio Ratio of non-zero to zero values in the matrix.
     * @return True if the matrix has at least (1 - ratio)*max_number_of_elements
     *         elements, which are not equal to zero.
     */
    bool is_efficient(double ratio) const override;

    /**
     * @brief Returns an iterator to the first non-zero element in the matrix.
     * @return An iterator to the first non-zero element in the matrix.
     */
    IteratorWrapper begin() const override;

    /**
     * @brief Returns an iterator past the last element of the matrix.
     * @return An iterator past the last element of the matrix.
     */
    IteratorWrapper end() const override;

  protected:

    /**
     * @brief Prints the matrix to the provided stream in a bracket format.
     *        No whitespace is printed past the matrix.
     * @param os Stream to print the matrix into.
     */
    void print(std::ostream & os) const override;

  private:

    /**
     * @brief A 2D vector serving as a container for the elements of the matrix.
     *        All elements, including the ones equal to zero, are stored.
     */
    std::vector<std::vector<double>> _data;
};