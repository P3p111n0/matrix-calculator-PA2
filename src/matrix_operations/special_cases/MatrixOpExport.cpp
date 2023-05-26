#include "MatrixOpExport.h"

MatrixOpExport::MatrixOpExport() : MatrixOpSpecial("EXPORT") {}

Matrix MatrixOpExport::evaluate(const std::vector<Matrix> &) const {
    return {0};
}
