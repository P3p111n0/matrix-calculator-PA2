#pragma once

#include "MatrixOpTwoArgs.h"

/**
 * @brief An operation representing matrix addition.
 */
class MatrixOpPlus : public MatrixOpTwoArgs {
  public:

    /**
     * @brief Initializes the operator with <b>priority = 1</b> and
     *        <b>name = "PLUS"</b>.
     */
    MatrixOpPlus();

    /**
     * @brief Performs matrix addition. Calls <b>Matrix::operator+</b>.
     * @param args An array of arguments for matrix addition. For
     *             requirements for <b>args</b>, see <b>MatrixOp::evaluate</b>.
     * @return Result of matrix addition.
     * @throws std::invalid_argument if <b>Matrix::operator+</b> throws.
     */
    Matrix evaluate(const std::vector<Matrix> & args) const override;
};
