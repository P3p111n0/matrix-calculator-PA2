#pragma once

#include "../matrix_wrapper/MatrixDimensions.h"
#include "../representations/DenseMatrix.h"
#include "AbstractMatrixIterator.h"
#include <vector>

/**
 * @brief Implements iterators for the DenseMatrix matrix representation.
 */
class DenseMatrixIterator : public AbstractMatrixIterator {
    using DenseMatrixContainer = std::vector<std::vector<double>>;

  public:
    /**
     * @brief Initializes the iterator.
     * @param ptr A pointer to the matrix to iterate over. Its data and
     *            a pointer to it's dimensions are extracted through this
     *            pointer.
     * @param row Row of the element, where the iterating will begin.
     * @param column Column of the element, where the iterating will begin.
     */
    DenseMatrixIterator(const DenseMatrix * ptr, std::size_t row,
                        std::size_t column);

    /**
     * @brief An alternative constructor. Instead of being extracted, the data
     *        container and dimensions are taken as an argument, especially
     *        useful for reading user input, as no DenseMatrix object needs to
     *        exist.
     * @param ptr Pointer to the dimensions of <b>data</b>
     * @param data A reference to the data container of matrix.
     * @param row Row of the starting element.
     * @param column Column of the starting element.
     */
    DenseMatrixIterator(const MatrixDimensions * ptr,
                        const DenseMatrixContainer & data, std::size_t row,
                        std::size_t column);

    /**
     * @brief Moves the iterator to the next non-zero element. Behavior is
     *        undefined if the iterator is already at the end of the range.
     */
    void operator++() override;

    /**
     * @brief Allow access to the element the iterator is currently pointing to.
     * @return Position and value of the current element wrapped in a
     *         <b>MatrixElement</b> struct.
     */
    MatrixElement operator*() const override;

    /**
     * @brief Calculates the number of non-zero elements between <b>this</b>
     *        and <b>dst</b>. Behavior is undefined if <b>dst</b> is not
     *        reachable from <b>this</b>.
     * @param dst Iterator to calculate the distance to.
     * @return Distance to <b>dst</b>.
     */
    std::size_t distance(const AbstractMatrixIterator & dst) const override;

  private:

    /**
     * @brief A const reference to the data container. A reference is used to
     *        prevent needless copies, of course there is risk of keeping
     *        a reference to dead data.
     */
    const DenseMatrixContainer & _data;

    /**
     * @brief A helper function for iterating, moves the iterator to the next
     *        column of the current row, or to the first column of the next
     *        row, if the iterator is pointing to the last column of the
     *        current row. Doesn't take values of elements into consideration.
     */
    void next_element();
};