#include "MatrixOpTranspose.h"

MatrixOpTranspose::MatrixOpTranspose()
    : MatrixOpSArg(3, "TRANSPOSE") {}

Matrix MatrixOpTranspose::evaluate(const std::vector<Matrix> & args) const {
    Matrix rhs = args[0];
    rhs.transpose();
    return rhs;
}
