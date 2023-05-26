#pragma once

#include "MatrixOpSArg.h"

/**
 * @brief An operation representing the calculation of the rank of a matrix.
 */
class MatrixOpRank : public MatrixOpSArg {
  public:

    /**
     * @brief Initializes the operation with <b>priority = 2</b> and
     *        <b>name = "RANK"</b>.
     */
    MatrixOpRank();

    /**
     * @brief Performs the calculation of the rank of a matrix. Calls
     *        <b>Matrix::rank()</b>.
     * @param args An array of arguments for rank calculation. For
     *             requirements for <b>args</b>, see <b>MatrixOp::evaluate</b>.
     * @return The rank of the matrix in a 1x1 matrix.
     */
    Matrix evaluate(const std::vector<Matrix> & args) const override;
};
