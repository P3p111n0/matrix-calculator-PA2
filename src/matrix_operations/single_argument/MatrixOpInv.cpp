#include "MatrixOpInv.h"

MatrixOpInv::MatrixOpInv()
    : MatrixOpSArg(3, "INV") {}

Matrix MatrixOpInv::evaluate(const std::vector<Matrix> & args) const {
    Matrix rhs = args[0];
    rhs.inverse();
    return rhs;
}
