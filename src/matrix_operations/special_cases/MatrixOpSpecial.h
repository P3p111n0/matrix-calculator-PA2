#pragma once

#include "../MatrixOp.h"

/**
 * @brief An abstract base class for special cases of matrix operations.
 *        All classes derived from <b>MatrixOpSpecial</b> only exist for the
 *        purposes of parsing and do nothing. Initializes
 *        <b>MatrixOp::arity = 0</b> and <b>MatrixOp::priority = 0</b>.
 */
class MatrixOpSpecial : public MatrixOp {
  protected:
    MatrixOpSpecial(const std::string & name) : MatrixOp(0, 0, name) {}
};