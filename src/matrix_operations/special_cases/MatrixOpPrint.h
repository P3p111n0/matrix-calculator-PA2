#pragma once

#include "../MatrixOp.h"

class MatrixOpPrint : public MatrixOp {
  public:
    MatrixOpPrint();
    Matrix evaluate(const std::vector<Matrix> &) const override;
};
