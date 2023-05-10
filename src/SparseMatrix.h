#pragma once

#include "MatrixMemoryRepr.h"
#include "MatrixElement.h"
#include <functional>
#include <initializer_list>
#include <map>
#include <vector>

class SparseMatrix : public MatrixMemoryRepr {
    friend class SparseMatrixIterator;
  public:
    /**
     * @brief Creates a zero-filled representation of a sparse matrix of the
     *        provided dimensions.
     * @param rows The amount of desired rows.
     * @param columns The amount of desired columns.
     * @throws std::invalid_argument if rows or columns are equal to zero.
     */
    SparseMatrix(size_t rows, size_t columns);

    /**
     * @brief Initializes the sparse matrix with the provided values.
     *        Dimensions are set by the number of rows and columns of the
     *        initializer list. No checks are ran on the provided values, so
     *        the resulting matrix may not be sparse, if the original matrix
     *        is dense.
     * @param initializerList An initializer list of values to load into
     *                        the matrix.
     * @throws std::invalid_argument if the amount of columns is not consistent
     *                               across all rows or if the initializer list
     *                               has zero rows or columns.
     */
    SparseMatrix(std::initializer_list<std::initializer_list<double>> initializerList);

    /**
     * @brief Creates a sparse matrix from a range determined by two iterators.
     *        No checks are performed on the range, so if the given range
     *        contains elements of a dense matrix, the resulting representation
     *        may not be effective.
     * @param begin The beginning of the given range.
     * @param end End of the given range.
     */
    SparseMatrix(IteratorWrapper begin, IteratorWrapper end);

    /**
     * @brief Returns a pointer to a dynamically allocated copy of the matrix.
     *        It is the programmer's responsibility to free this pointer.
     * @return A pointer to the dynamically allocated copy.
     */
    MatrixMemoryRepr * clone() const override;

    /**
     * @brief Returns the element at the given indices, or an empty optional
     *        object, if the indices exceed the dimensions of the matrix.
     *        Standard zero-based indexing is presumed.
     * @param row Row of the element in question.
     * @param column Column of the element in question.
     * @return Value at the given indices, or an empty optional object, if the
     *         indices exceed the dimensions of the matrix.
     */
    std::optional<double> at(std::size_t row, std::size_t column) const override;

    /**
     * @brief Increases the element's value at the given indices by @value
     *        using operator+. Standard zero-based indexing is presumed.
     *        No guarantees are provided regarding memory efficiency when using
     *        this method.
     * @param row Row of the element in question.
     * @param column Column of the element in question.
     * @param value The value to add to the given element.
     * @throws std::out_of_range if the indices exceed the dimensions of the
     *                           matrix.
     */
    void add(std::size_t row, std::size_t column, double value) override;

    /**
     * @brief Changes the element at the given indices to @value. Standard
     *        zero-based indexing is presumed. Memory efficiency cannot be
     *        guaranteed when using this method. If the represented matrix
     *        becomes a dense one because of this method, this event won't be
     *        recognized and the representation will be inefficient.
     * @param row Row of the given element.
     * @param column Column of the given element.
     * @param value Value, which will replace the element at the given indices.
     * @throws std::out_of_range if the indices exceed the dimensions of the
     *                           matrix.
     */
    void modify(std::size_t row, std::size_t column, double value) override;

    /**
     * @brief Swaps elements in @first_row and @second_row. Standard zero-based
     *        indexing is presumed.
     * @param first_row Index of the row to swap with second_row.
     * @param second_row Index of the row to swap with first_row.
     * @throws std::out_of_range If at least one the indices exceeds the
     *                           dimensions of the matrix.
     */
    void swap_rows(std::size_t first_row, std::size_t second_row) override;

    /**
     * @brief Determines, whether the representation as a sparse matrix is
     *        effective for the given ratio.
     * @param ratio The ratio to determine the efficiency of the representation.
     * @return True if the matrix has at least ratio*maximum_possible_elements
     *         zeroes, false otherwise.
     */
    bool is_efficient(double ratio) const override;

    /**
     * @brief Returns an iterator to the first non-zero element of the matrix.
     * @return An iterator to the first non-zero element of the matrix.
     */
    IteratorWrapper begin() const override;

    /**
     * @brief Returns an iterator past the last non-zero element of the matrix.
     * @return An iterator past the last element of the matrix.
     */
    IteratorWrapper end() const override;

  protected:

    /**
     * @brief Prints the matrix to the provided output stream in a bracket
     *        format. No whitespace is printed at the end of the matrix.
     * @param os Stream to print the matrix into.
     */
    void print(std::ostream & os) const override;

  private:

    /**
     * @brief A container for the values of the represented matrix.
     *        Key - A Position struct with the row and column of the given value.
     *              Values are sorted primarily by their rows and by their
     *              columns, if such a comparison would be insufficient.
     *        Value - The value at in the row and column. Only non-zero values
     *                are present.
     */
     std::map<Position, double> _data;
};