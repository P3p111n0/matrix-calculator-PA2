#include "MatrixOpPrint.h"

MatrixOpPrint::MatrixOpPrint() : MatrixOp(1, 0, "PRINT") {}

Matrix MatrixOpPrint::evaluate(const std::vector<Matrix> &) const {
    return {0};
}
