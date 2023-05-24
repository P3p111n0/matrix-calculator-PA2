#pragma once

#include "MatrixOpSArg.h"

class MatrixOpRank : public MatrixOpSArg {
  public:
    MatrixOpRank();
    Matrix evaluate(const std::vector<Matrix> &) const override;
};
