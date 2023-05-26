#pragma once

#include "MatrixOpTwoArgs.h"

/**
 * @brief An operation representing matrix unification.
 */
class MatrixOpUnite : public MatrixOpTwoArgs {
  public:

    /**
     * @brief Initializes the operator with <b>priority = 3</b> and
     *        <b>name = "UNITE"</b>.
     */
    MatrixOpUnite();

    /**
     * @brief Performs matrix unification. Calls
     *        <b>Matrix::unite(const Matrix &, const Matrix &)</b>.
     * @param args An array of arguments for matrix unification.. For
     *             requirements for <b>args</b>, see <b>MatrixOp::evaluate</b>.
     * @return Result of matrix unification.
     * @throws std::invalid_argument if <b>Matrix::unite(const Matrix &,
     *                               const Matrix &)</b> throws.
     */
    Matrix evaluate(const std::vector<Matrix> & args) const override;
};
