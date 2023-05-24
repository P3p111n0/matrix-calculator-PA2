#pragma once

#include "MatrixOpSArg.h"

class MatrixOpGauss : public MatrixOpSArg {
  public:
    MatrixOpGauss();
    Matrix evaluate(const std::vector<Matrix> & args) const override;
};
