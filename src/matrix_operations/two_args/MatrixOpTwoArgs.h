#pragma once

#include "../MatrixOp.h"

/**
 * @brief A base class for operations with arity equal to 2. It's only purpose
 *        is to initialize the <b>arity</b> field of base class <b>MatrixOp</b>.
 */
class MatrixOpTwoArgs : public MatrixOp {
  protected:
    MatrixOpTwoArgs(std::size_t priority, const std::string & name)
        : MatrixOp(2, priority, name) {}
};
