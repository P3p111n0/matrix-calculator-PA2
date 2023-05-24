#include "MatrixOpExport.h"

MatrixOpExport::MatrixOpExport() : MatrixOp(1, 0, "EXPORT") {}

Matrix MatrixOpExport::evaluate(const std::vector<Matrix> &) const {
    return {0};
}
