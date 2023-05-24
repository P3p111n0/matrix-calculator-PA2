#pragma once

#include "MatrixOpTwoArgs.h"

class MatrixOpMul : public MatrixOpTwoArgs {
  public:
    MatrixOpMul();
    Matrix evaluate(const std::vector<Matrix> &) const override;
};
