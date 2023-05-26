#pragma once

#include "../MatrixOp.h"

/**
 * @brief An abstract base class for operations with 5 arguments. Initializes
 *        <b>MatrixOp::arity</b> to 5.
 */
class MatrixOpFiveArgs : public MatrixOp {
  protected:
    MatrixOpFiveArgs(std::size_t priority, const std::string & name)
        : MatrixOp(5, priority, name) {}
};