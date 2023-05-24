#pragma once

#include "MatrixOpSArg.h"

class MatrixOpTranspose : public MatrixOpSArg {
  public:
    MatrixOpTranspose();
    Matrix evaluate(const std::vector<Matrix> &) const override;
};
