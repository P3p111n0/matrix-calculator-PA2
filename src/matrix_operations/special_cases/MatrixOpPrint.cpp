#include "MatrixOpPrint.h"

MatrixOpPrint::MatrixOpPrint() : MatrixOpSpecial("PRINT") {}

Matrix MatrixOpPrint::evaluate(const std::vector<Matrix> &) const {
    return {0};
}
