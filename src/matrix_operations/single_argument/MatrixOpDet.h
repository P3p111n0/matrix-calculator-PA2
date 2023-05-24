#pragma once

#include "MatrixOpSArg.h"

class MatrixOpDet : public MatrixOpSArg {
  public:
    MatrixOpDet();
    Matrix evaluate(const std::vector<Matrix> &) const override;
};
