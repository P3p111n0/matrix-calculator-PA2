#pragma once

#include "MatrixOpFiveArgs.h"

/**
 * @brief An operation representing sub-matrix extraction.
 */
class MatrixOpCut : public MatrixOpFiveArgs {
  public:

    /**
     * @brief Initializes the operator to <b>priority = 3</b> and
     *        <b>name = "CUT"</b>.
     */
    MatrixOpCut();

    /**
     * @brief Performs the sub-matrix extraction.
     *        Calls <b>Matrix::cut(const Matrix &, const Matrix &,
     *        const Matrix &, const Matrix &)</b>.
     * @param args An array of arguments for sub-matrix extraction. For
     *             requirements for <b>args</b>, see <b>MatrixOp::evaluate()</b>.
     * @return The extracted sub-matrix.
     * @throws std::invalid_argument if <b>Matrix::cut(const Matrix &,
     *                               const Matrix &, const Matrix &,
     *                               const Matrix &)</b> throws.
     */
    Matrix evaluate(const std::vector<Matrix> & args) const override;
};
