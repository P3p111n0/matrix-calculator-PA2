#pragma once

#include "MatrixOpFiveArgs.h"

class MatrixOpCut : public MatrixOpFiveArgs {
  public:
    MatrixOpCut();
    Matrix evaluate(const std::vector<Matrix> &) const override;
};
