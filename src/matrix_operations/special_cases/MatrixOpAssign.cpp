#include "MatrixOpAssign.h"

MatrixOpAssign::MatrixOpAssign() : MatrixOp(2, 0, "=") {}

Matrix MatrixOpAssign::evaluate(const std::vector<Matrix> &) const {
    return {0};
}
