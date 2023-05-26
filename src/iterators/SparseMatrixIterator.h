#pragma once

#include "../representations/SparseMatrix.h"
#include "AbstractMatrixIterator.h"
#include <map>

/**
 * @brief Implements iterators for the SparseMatrix matrix representation.
 */
class SparseMatrixIterator : public AbstractMatrixIterator {
    using MapIterator = std::map<Position, double>::const_iterator;

  public:

    /**
     * @brief Initializes the iterator.
     * @param ptr A pointer to the matrix into which the iterator will point.
     *            A pointer to the dimensions of the matrix and an iterator to
     *            the end of it's data container are extracted from this pointer.
     * @param current_iterator An iterator to the first element of the
     *                         desired range.
     */
    SparseMatrixIterator(const SparseMatrix * ptr,
                         const MapIterator & current_iterator);

    /**
     * @brief An alternative constructor. Instead of being extracted, the
     *        pointer to the dimensions and the iterator to the end are
     *        taken as arguments. Useful when reading user input, as no
     *        SparseMatrix object needs to exist.
     * @param ptr A pointer to the dimensions of the matrix.
     * @param current_iterator An iterator to the start of the desired range.
     * @param end_iterator An iterator to the end of the container of the matrix.
     */
    SparseMatrixIterator(const MatrixDimensions * ptr,
                         const MapIterator & current_iterator,
                         const MapIterator & end_iterator);

    /**
     * @brief Moves the iterator to the next non-zero element of the matrix.
     *        Behavior is undefined if the iterator is already pointing to the
     *        last non-zero element.
     */
    void operator++() override;

    /**
     * @brief Allows access to the element the iterator is currently pointing to.
     *        Behavior is undefined if the iterator is already pointing to the
     *        end of the matrix.
     * @return Position and value of the current element wrapped in a
     *         <b>MatrixElement</b> struct.
     */
    MatrixElement operator*() const override;

    /**
     * @brief Calculates the number of non-zero element between <b>this</b> and
     *        <b>dst</b>. Behavior is undefined if <b>dst</b> is unreachable
     *        from <b>this</b>.
     * @param dst An iterator to calculate the distance to.
     * @return Distance to <b>dst</b>.
     */
    std::size_t distance(const AbstractMatrixIterator & dst) const override;

  private:

    /**
     * @brief Iterator to the current element. Points into
     *        <b>SparseMatrix::_data</b>.
     */
    MapIterator _it;

    /**
     * @brief A copy of <b>SparseMatrix::_data.end()</b>. Used for controlling
     *        behavior when setting end states.
     */
    MapIterator _end;
};
