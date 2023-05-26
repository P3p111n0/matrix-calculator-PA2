#include "MatrixOpImport.h"

MatrixOpImport::MatrixOpImport() : MatrixOpSpecial("IMPORT") {}

Matrix MatrixOpImport::evaluate(const std::vector<Matrix> &) const {
    return {0};
}
