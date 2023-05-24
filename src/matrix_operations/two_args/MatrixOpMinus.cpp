#include "MatrixOpMinus.h"

MatrixOpMinus::MatrixOpMinus() : MatrixOpTwoArgs(1, "MINUS") {}

Matrix MatrixOpMinus::evaluate(const std::vector<Matrix> & args) const {
    const Matrix & rhs = args[0];
    const Matrix & lhs = args[1];
    return lhs - rhs;
}
