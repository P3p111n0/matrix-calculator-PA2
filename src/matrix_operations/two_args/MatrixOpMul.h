#pragma once

#include "MatrixOpTwoArgs.h"

/**
 * @brief An operation representing matrix multiplication.
 */
class MatrixOpMul : public MatrixOpTwoArgs {
  public:

    /**
     * @brief Initializes the operator with <b>priority = 5</b> and
     *        <b>name = "MUL"</b>.
     */
    MatrixOpMul();

    /**
     * @brief Performs matrix multiplication. Calls
     *        <b>Matrix::operator*(const Matrix &, const Matrix &)</b>.
     * @param args An array of arguments for matrix multiplication. For
     *             requirements regarding <b>args</b>, see
     *             <b>MatrixOp::evaluate</b>.
     * @return Product of the two matrices.
     * @throws std::invalid_argument if <b>Matrix::operator*(const Matrix &,
     *                               const Matrix &)</b> throws.
     */
    Matrix evaluate(const std::vector<Matrix> & args) const override;
};
