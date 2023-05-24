#pragma once

#include "../MatrixOp.h"

class MatrixOpAssign : public MatrixOp {
  public:
    MatrixOpAssign();
    Matrix evaluate(const std::vector<Matrix> &) const override;
};
