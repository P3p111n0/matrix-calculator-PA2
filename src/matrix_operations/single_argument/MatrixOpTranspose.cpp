#include "MatrixOpTranspose.h"

MatrixOpTranspose::MatrixOpTranspose()
    : MatrixOpSArg(3, "TRANSPOSE") {}

Matrix MatrixOpTranspose::evaluate(const std::vector<Matrix> & args) const {
    const Matrix & rhs = args[0];
    return rhs.transpose();
}
