#pragma once

#include "MatrixOpTwoArgs.h"

class MatrixOpPlus : public MatrixOpTwoArgs {
  public:
    MatrixOpPlus();
    Matrix evaluate(const std::vector<Matrix> &) const override;
};
