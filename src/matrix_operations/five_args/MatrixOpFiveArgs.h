#pragma once

#include "../MatrixOp.h"

class MatrixOpFiveArgs : public MatrixOp {
  protected:
    MatrixOpFiveArgs(std::size_t priority, const std::string & name)
        : MatrixOp(5, priority, name) {}
};