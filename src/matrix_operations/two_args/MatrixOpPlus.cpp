#include "MatrixOpPlus.h"

MatrixOpPlus::MatrixOpPlus() : MatrixOpTwoArgs(1, "PLUS") {}

Matrix MatrixOpPlus::evaluate(const std::vector<Matrix> & args) const {
    const Matrix & rhs = args[0];
    const Matrix & lhs = args[1];
    return lhs + rhs;
}
