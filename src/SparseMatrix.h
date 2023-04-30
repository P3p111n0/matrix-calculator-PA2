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
     *                               across all rows.
     */
    SparseMatrix(std::initializer_list<std::initializer_list<double>> initializerList);
    SparseMatrix(IteratorWrapper, IteratorWrapper);
    MatrixMemoryRepr * clone() const override;
    std::optional<double> at(std::size_t, std::size_t) const override;
    void add(std::size_t, std::size_t, double) override;
    void modify(std::size_t, std::size_t, double) override;
    void swap_rows(std::size_t, std::size_t) override;

    /**
     * @brief Determines, whether the representation as a sparse matrix is
     *        effective for the given ratio.
     * @param ratio The ratio to determine the efficiency of the representation.
     * @return True if the matrix has at least ratio*maximum_possible_elements
     *         zeroes, false otherwise.
     */
    bool is_efficient(double ratio) const override;
    IteratorWrapper begin() const override;
    IteratorWrapper end() const override;

  protected:
    void print(std::ostream &) const override;

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