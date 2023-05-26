#pragma once

#include "../MatrixOp.h"

/**
 * @brief A base class for single argument operations. Initializes the
 *        <b>arity</b> field with 1.
 */
class MatrixOpSArg : public MatrixOp {
  protected:
    MatrixOpSArg(std::size_t priority, const std::string & name)
        : MatrixOp(1, priority, name) {}
};
