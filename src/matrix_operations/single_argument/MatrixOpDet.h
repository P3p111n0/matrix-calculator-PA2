#pragma once

#include "MatrixOpSArg.h"

/**
 * @brief An operation representing the determinant of a matrix.
 */
class MatrixOpDet : public MatrixOpSArg {
  public:

    /**
     * @brief Initializes the operator with <b>priority = 2</b> and
     *        <b>name = "DET"</b>.
     */
    MatrixOpDet();

    /**
     * @brief Performs the calculation of the determinant of a matrix. Calls
     *        <b>Matrix::det()</b>.
     * @param args An array of arguments for determinant calculation. For
     *             requirements for <b>args</b>, see <b>MatrixOp::evaluate</b>.
     * @return The determinant of the matrix in a 1x1 matrix.
     * @throws std::invalid_argument if the matrix is non-square.
     */
    Matrix evaluate(const std::vector<Matrix> & args) const override;
};
