#pragma once

#include "MatrixOpSArg.h"

class MatrixOpInv : public MatrixOpSArg {
  public:
    MatrixOpInv();
    Matrix evaluate(const std::vector<Matrix> &) const override;
};
