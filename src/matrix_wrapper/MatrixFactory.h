#pragma once

#include "../iterators/IteratorWrapper.h"
#include "../representations/MatrixMemoryRepr.h"
#include <vector>

/**
 * @brief A factory class which implements picking effective representations
 *        of matrices based on their contents and transitions between these
 *        representations.
 */
class MatrixFactory {
  public:
    MatrixFactory() = delete;

    /**
     * @brief Creates a MatrixFactory with the provided ratio.
     * @param sparse_ratio Ratio of zeroes to the non-zero elements in a matrix.
     *                     This ratio determines the final representation of the
     *                     matrix.
     */
    explicit MatrixFactory(double sparse_ratio);

    /**
     * @brief Creates a representation for a zero filled of the given
     * dimensions. The returned pointer is dynamically allocated, deleting this
     *        pointer is the responsibility of the programmer.
     * @param rows Number of desired rows in the matrix.
     * @param columns Number of desired columns in the matrix.
     * @return A pointer to a dynamically allocated representation of the
     *         resulting matrix.
     * @throws std::invalid_argument If the constructor of the given
     *                               representation throws. See their
     *                               documentation for more information.
     */
    MatrixMemoryRepr * get_initial_repr(std::size_t rows,
                                        std::size_t columns) const;

    /**
     * @brief Creates an efficient representation for a matrix initialized with
     *        the provided initializer_list. If the initializer list has at
     *        least 'ratio*num_of_rows_of_init*num_of_columns_of_init' elements
     *        equal to zero, the values will be represented in a sparse matrix,
     *        or a dense matrix otherwise. The final representation is
     *        dynamically allocated, it is up to the programmer to delete it.
     * @param init An initializer list used to create the representation.
     * @return A pointer to a heap allocated matrix representation filled with
     *         the provided values.
     * @throws std::invalid_argument If the constructor of the appropriate
     *                               representation from an initializer list
     *                               throws. See their documentation for more
     *                               information.
     */
    MatrixMemoryRepr * get_initial_repr(
        std::initializer_list<std::initializer_list<double>> init) const;

    /**
     * @brief Creates a representation of a 1x1 matrix containing only the
     *        single provided value. This matrix is always represented as a
     *        sparse matrix. The final representation is heap allocated, it is
     *        up to the programmer to delete it.
     * @param value Value, which will be represented in the matrix.
     * @return A pointer to the heap allocated representation the a sparse
     *         matrix.
     * @throws std::invalid_argument If the constructor
     *                               SparseMatrix(rows, columns) throws. See
     *                               its documentation for more information.
     */
    MatrixMemoryRepr * get_initial_repr(double value) const;

    /**
     * @brief Creates a memory effective representation of a matrix from the
     *        range determined by the provided iterators. The resulting
     *        representation is dynamically allocated, it's the programmer's
     *        responsibility to delete it.
     * @param begin An iterator determining the start of the range, from which
     *              the representation will be constructed.
     * @param end An iterator to the end of the given range.
     * @return A pointer to a dynamically allocated representation of the
     *         matrix determined by the given range.
     */
    MatrixMemoryRepr * get_initial_repr(IteratorWrapper begin,
                                        IteratorWrapper end) const;

    /**
     * @brief Converts a matrix representation to a different one, if the
     *        current representation is inefficient given in the given ratio.
     *        The efficiency is determined by the is_efficient(ratio)
     *        method of the representation. May return the repr_to_convert
     *        if no conversion is necessary. Conversion is handled by iterating
     *        over the representation using it's begin() and end() methods.
     *        If the representation is converted, the returned representation
     *        is heap allocated, it's up to the programmer to delete it.
     * @param repr_to_convert Pointer to the representation to convert.
     * @return repr_to_convert if no conversion is needed, a memory efficient
     *         copy of the provided representation otherwise.
     */
    MatrixMemoryRepr * convert(MatrixMemoryRepr * repr_to_convert) const;

    /**
     * @brief Returns the ratio provided in the constructor.
     * @return Ratio used to determine efficiency of matrix representations.
     */
    double ratio() const;

  private:
    /**
     * @brief Ratio used to judge memory efficiency of representations.
     *        A matrix is determined to be sparse, if it has at least
     *        (1 - ratio)*rows*columns elements equal to zero.
     */
    double _ratio;
};
