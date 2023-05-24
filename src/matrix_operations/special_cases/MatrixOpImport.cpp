#include "MatrixOpImport.h"

MatrixOpImport::MatrixOpImport() : MatrixOp(1, 0, "IMPORT") {}

Matrix MatrixOpImport::evaluate(const std::vector<Matrix> &) const {
    return {0};
}
