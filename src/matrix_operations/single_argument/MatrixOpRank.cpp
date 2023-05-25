#include "MatrixOpRank.h"

MatrixOpRank::MatrixOpRank() : MatrixOpSArg(2, "RANK") {}

Matrix MatrixOpRank::evaluate(const std::vector<Matrix> & args) const {
    const Matrix & rhs = args[0];
    auto rank = rhs.rank();
    return Matrix(static_cast<double>(rank));
}
