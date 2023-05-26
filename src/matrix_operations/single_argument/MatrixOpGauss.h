#pragma once

#include "MatrixOpSArg.h"

/**
 * @brief An operation representing Gaussian elimination to row echelon form.
 */
class MatrixOpGauss : public MatrixOpSArg {
  public:

    /**
     * @brief Initializes the operator with <b>priority = 3</b> and
     *        <b>name = "GAUSS"</b>.
     */
    MatrixOpGauss();

    /**
     * @brief Performs gaussian elimination to row echelon form of the matrix.
     *        calls <b>Matrix::gem()</b>
     * @param args An array of arguments for gaussian elimination. For
     *             requirements for <b>args</b>, see <b>MatrixOp::evaluate</b>.
     * @return The matrix eliminated to row echelon form.
     */
    Matrix evaluate(const std::vector<Matrix> & args) const override;
};
