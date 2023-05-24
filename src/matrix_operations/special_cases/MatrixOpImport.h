#pragma once

#include "../MatrixOp.h"

class MatrixOpImport : public MatrixOp {
  public:
    MatrixOpImport();
    Matrix evaluate(const std::vector<Matrix> &) const override;
};
