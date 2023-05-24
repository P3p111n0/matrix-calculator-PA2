#pragma once

#include "../MatrixOp.h"

class MatrixOpSArg : public MatrixOp {
  protected:
    MatrixOpSArg(std::size_t priority, const std::string & name)
        : MatrixOp(1, priority, name) {}
};
