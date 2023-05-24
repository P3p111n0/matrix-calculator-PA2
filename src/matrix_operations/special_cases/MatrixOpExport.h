#pragma once

#include "../MatrixOp.h"

class MatrixOpExport : public MatrixOp {
  public:
    MatrixOpExport();
    Matrix evaluate(const std::vector<Matrix> &) const override;
};
