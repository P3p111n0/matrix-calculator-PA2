#pragma once

#include "../MatrixOp.h"

/**
 * @brief Represents a special case for matrix operations. Classes in the
 *        <b>special_cases</b> directory do nothing, they only exist for
 *        the purposes of parsing.
 */
class MatrixOpAssign : public MatrixOp {
  public:

    /**
     * @brief Initializes the operator to <b>arity = 2</b>, <b>priority = 0</b>
     *        and <b>name = "="</b>.
     */
    MatrixOpAssign();

    /**
     * @brief Only exists for the purposes of parsing.
     * @return Value 0 in a 1x1 matrix regardless of it's arguments.
     */
    Matrix evaluate(const std::vector<Matrix> &) const override;
};
