#pragma once

#include "../matrix_wrapper/MatrixDimensions.h"
#include "../matrix_wrapper/MatrixElement.h"

/**
 * @brief An abstract base class for matrix iterators. Is used in class
 *        <b>IteratorWrapper</b>.
 */
class AbstractMatrixIterator {
  public:

    /**
     * @brief Initializes the iterator.
     * @param ptr Pointer to a <b>MatrixDimensions</b> struct. This pointer is
     *            used for iterator comparison, meaning it needs to be unique for
     *            each matrix.
     * @param row Row of the current element, which the iterator is pointing to.
     * @param column Column of the current element, which the iterator is
     *               pointing to.
     */
    AbstractMatrixIterator(const MatrixDimensions * ptr, std::size_t row,
                           std::size_t column);
    virtual ~AbstractMatrixIterator() = default;

    /**
     * @brief A preincrement operator. Moves the iterator to the next non-zero
     *        element of the matrix. Behavior is undefined if the
     *        iterator is already pointing to the column of the last row.
     */
    virtual void operator++() = 0;

    /**
     * @brief A dereference operator. Allows access to the element the iterator
     *        is currently pointing to. Behavior is undefined if the iterator
     *        is pointing to the end of the matrix or past it.
     * @return Position and value of the element the iterator is currently
     *         pointing to, wrapped in a <b>MatrixElement</b> struct.
     */
    virtual MatrixElement operator*() const = 0;

    /**
     * @brief Compares two iterators for equality by their
     *        <b>MatrixDimensions</b> pointers and current position.
     * @param rhs Iterator to compare against.
     * @return True if their pointers, row and column compare equal, false
     *         otherwise.
     */
    bool operator==(const AbstractMatrixIterator & rhs) const;

    /**
     * @brief Compares two iterators for inequality. Calls
     *        <b>AbstractMatrixIterator::operator==(
     *        const AbstractMatrixIterator &)</b>.
     * @param rhs Iterator to compare against.
     * @return True if <b>AbstractMatrixIterator::operator==(
     *         const AbstractMatrixIterator &)</b> returns false, false
     *         otherwise.
     */
    bool operator!=(const AbstractMatrixIterator & rhs) const;

    /**
     * @brief Calculates the distance best <b>this</b> and <b>dst</b>, possibly
     *        repeatedly incrementing. Behavior is undefined if <b>dst</b>
     *        is not reachable from <b>this</b>
     * @param dst Iterator to measure the distance to.
     * @return Number of non-zero elements between <b>this</b> and <b>dst</b>.
     */
    virtual std::size_t distance(const AbstractMatrixIterator & dst) const = 0;

    /**
     * @brief A getter for the number of rows of the matrix the iterator
     *        is pointing into.
     * @return Number of rows of the matrix the iterator is iterating over.
     */
    std::size_t get_matrix_rows() const;

    /**
     * @brief A getter for the number of columns of the matrix the iterator
     *        is pointing into.
     * @return Number of columns of the matrix the iterator is iterating over.
     */
    std::size_t get_matrix_columns() const;

  protected:

    /**
     * @brief A pointer to dimensions of the matrix the iterator is iterating
     *        over. This pointer <b>has to be</b> unique for every matrix,
     *        else equality and inequality operators will break.
     */
    const MatrixDimensions * _ptr;

    /**
     * @brief Row of the current element being pointed at.
     */
    std::size_t _row;

    /**
     * @brief Column of the current element being pointed at.
     */
    std::size_t _column;
};
