#pragma once

#include "MatrixOpTwoArgs.h"

class MatrixOpMinus : public MatrixOpTwoArgs {
  public:
    MatrixOpMinus();
    Matrix evaluate(const std::vector<Matrix> &) const override;
};
