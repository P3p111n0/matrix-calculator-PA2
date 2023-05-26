#pragma once

#include "MatrixOpTwoArgs.h"

/**
 * @brief An operation representing matrix subtraction.
 */
class MatrixOpMinus : public MatrixOpTwoArgs {
  public:
    /**
     * @brief Initializes the operation with <b>priority = 1</b> and
     *        <b>name = "MINUS"</b>.
     */
    MatrixOpMinus();

    /**
     * @brief Performs matrix subtraction. Calls <b>Matrix::operator-</b>.
     * @param args An array of arguments for matrix subtraction. For
     *             requirements for <b>args</b>, see <b>MatrixOp::evaluate</b>.
     * @return Result of matrix subtraction.
     * @throws std::invalid_argument if <b>Matrix::operator-</b> throws.
     */
    Matrix evaluate(const std::vector<Matrix> & args) const override;
};
