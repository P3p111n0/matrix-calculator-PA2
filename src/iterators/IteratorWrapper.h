#pragma once

#include "AbstractMatrixIterator.h"
#include <memory>

/**
 * @brief A wrapper around AbstractMatrixIterator and derived classes. Enables
 *        polymorphic interaction and simplifies working with the iterators.
 */
class IteratorWrapper {
  public:

    /**
     * @brief Initializes the wrapper.
     * @param it A pointer to a heap allocated iterator.
     */
    IteratorWrapper(AbstractMatrixIterator * it);

    /**
     * @brief Moves the iterator to the next non-zero element. Calls
     *        <b>AbstractMatrixIterator::operator++()</b>.
     */
    void operator++();

    /**
     * @brief Allows access to the current element. Calls
     *        <b>AbstractMatrixIterator::operator*()</b>.
     * @return See <b>AbstractMatrixIterator::operator*()</b>.
     */
    MatrixElement operator*() const;

    /**
     * @brief Calculates the amount of non-zero elements between <b>this</b>
     *        and <b>dst</b>. Calls <b>AbstractMatrixIterator::distance(
     *        const AbstractMatrixIterator &)</b>.
     * @param dst Iterator to calculate the distance to.
     * @return See <b>AbstractMatrixIterator::distance(
     *         const AbstractMatrixIterator &)</b>.
     */
    std::size_t distance(const IteratorWrapper & dst) const;

    /**
     * @brief Compares two iterators for equality. Calls
     *        <b>AbstractMatrixIterator::operator==(
     *        const AbstractMatrixIterator &)</b>.
     * @param rhs Iterator to compare against.
     * @return See <b>AbstractMatrixIterator::operator==(
     *              const AbstractMatrixIterator &)</b>.
     */
    bool operator==(const IteratorWrapper & rhs) const;

    /**
     * @brief Compares two iterators for inequality. Calls
     *        <b>AbstractMatrixIterator::operator!=(
     *        const AbstractMatrixIterator &)</b>.
     * @param rhs Iterator to compare against.
     * @return See <b>AbstractMatrixIterator::operator!=(
     *              const AbstractMatrixIterator &)</b>.
     */
    bool operator!=(const IteratorWrapper & rhs) const;

    /**
     * @brief Getter for the number of rows of the matrix, to which the iterator
     *        is pointing. Calls
     *        <b>AbstractMatrixIterator::get_matrix_rows()</b>.
     * @return See <b>AbstractMatrixIterator::get_matrix_rows()</b>.
     */
    std::size_t get_matrix_rows() const;

    /**
     * @brief Getter for the number of columns of the matrix, to which the
     *        iterator is pointing. Calls
     *        <b>AbstractMatrixIterator::get_matrix_columns()</b>.
     * @return See <b>AbstractMatrixIterator::get_matrix_columns()</b>.
     */
    std::size_t get_matrix_columns() const;

  private:

    /**
     * @brief A pointer to the wrapped iterator. Allows polymorphic interaction.
     */
    std::unique_ptr<AbstractMatrixIterator> _iterator;
};