#pragma once

#include "MatrixOpSArg.h"

/**
 * @brief An operation representing the calculation of an inverse matrix.
 */
class MatrixOpInv : public MatrixOpSArg {
  public:

    /**
     * @brief Initializes the operator with <b>priority = 3</b> and
     *        <b>name = "INV"</b>.
     */
    MatrixOpInv();

    /**
     * @brief Performs the calculation of an inverse matrix. Calls
     *        <b>Matrix::inverse()</b>.
     * @param args An array of arguments for the calculation of an inverse
     *             matrix. For requirements for <b>args</b>, see
     *             <b>MatrixOp::evaluate</b>.
     * @return A matrix inverse to the matrix provided in <b>args</b>.
     * @throws std::invalid_argument if <b>Matrix::inverse()</b> throws.
     */
    Matrix evaluate(const std::vector<Matrix> & args) const override;
};
