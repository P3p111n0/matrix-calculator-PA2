#include "MatrixOpMul.h"

MatrixOpMul::MatrixOpMul() : MatrixOpTwoArgs(5, "MUL") {}

Matrix MatrixOpMul::evaluate(const std::vector<Matrix> & args) const {
    const Matrix & rhs = args[0];
    const Matrix & lhs = args[1];
    return lhs * rhs;
}
