#pragma once

#include "MatrixOpSArg.h"

/**
 * @brief An operation representing matrix transposition.
 */
class MatrixOpTranspose : public MatrixOpSArg {
  public:

    /**
     * @brief Initializes the operator with <b>priority = 3</b> and
     *        <b>name = "TRANSPOSE"</b>.
     */
    MatrixOpTranspose();

    /**
     * @brief Performs the matrix transposition. Calls <b>Matrix::transpose()</b>.
     * @param args An array of arguments for matrix transposition. For
     *             requirements for <b>args</b>, see <b>MatrixOp::evaluate</b>.
     * @return The matrix present in <b>args</b> transposed.
     */
    Matrix evaluate(const std::vector<Matrix> & args) const override;
};
