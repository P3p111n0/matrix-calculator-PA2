#pragma once

#include "MatrixOpTwoArgs.h"

class MatrixOpUnite : public MatrixOpTwoArgs {
  public:
    MatrixOpUnite();
    Matrix evaluate(const std::vector<Matrix> &) const override;
};
