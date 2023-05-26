#pragma once

#include "MatrixOpSpecial.h"

/**
 * @brief Represents a special case for matrix operations. Does nothing,
 *        only exists for the purposes of parsing.
 */
class MatrixOpPrint : public MatrixOpSpecial {
  public:

    /**
     * @brief Initializes the operation to <b>name = "PRINT"</b>.
     */
    MatrixOpPrint();

    /**
     * @brief Only exists for the purposes of parsing.
     * @return Value 0 in a 1x1 matrix regardless of it's arguments.
     */
    Matrix evaluate(const std::vector<Matrix> &) const override;
};
