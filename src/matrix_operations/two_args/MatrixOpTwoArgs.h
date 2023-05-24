#pragma once

#include "../MatrixOp.h"

class MatrixOpTwoArgs : public MatrixOp {
  protected:
    MatrixOpTwoArgs(std::size_t priority, const std::string & name)
        : MatrixOp(2, priority, name) {}
};
