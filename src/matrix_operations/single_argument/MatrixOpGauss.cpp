#include "MatrixOpGauss.h"

MatrixOpGauss::MatrixOpGauss() : MatrixOpSArg(3, "GAUSS") {}

Matrix MatrixOpGauss::evaluate(const std::vector<Matrix> & args) const {
    Matrix rhs = args[0];
    rhs.gem();
    return rhs;
}
