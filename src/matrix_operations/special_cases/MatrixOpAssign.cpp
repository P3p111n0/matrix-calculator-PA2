#include "MatrixOpAssign.h"

MatrixOpAssign::MatrixOpAssign() : MatrixOpSpecial("=") {}

Matrix MatrixOpAssign::evaluate(const std::vector<Matrix> &) const {
    return {0};
}
