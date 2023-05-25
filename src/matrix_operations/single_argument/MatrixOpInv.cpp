#include "MatrixOpInv.h"

MatrixOpInv::MatrixOpInv()
    : MatrixOpSArg(3, "INV") {}

Matrix MatrixOpInv::evaluate(const std::vector<Matrix> & args) const {
    const Matrix & rhs = args[0];
    return rhs.inverse();
}
