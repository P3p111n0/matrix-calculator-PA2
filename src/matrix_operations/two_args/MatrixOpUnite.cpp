#include "MatrixOpUnite.h"

MatrixOpUnite::MatrixOpUnite() : MatrixOpTwoArgs(3, "UNITE") {}

Matrix MatrixOpUnite::evaluate(const std::vector<Matrix> & args) const {
    const Matrix & rhs = args[0];
    const Matrix & lhs = args[1];
    return Matrix::unite(lhs, rhs);
}
