#include "MatrixOpDet.h"

MatrixOpDet::MatrixOpDet() : MatrixOpSArg(2, "DET") {}

Matrix MatrixOpDet::evaluate(const std::vector<Matrix> & args) const {
    Matrix rhs = args[0];
    auto det = rhs.det();
    if (!det.has_value()){
        throw std::invalid_argument("Determinant is undefined for non-square matrices.");
    }
    double det_val = det.value();
    return {det_val};
}
