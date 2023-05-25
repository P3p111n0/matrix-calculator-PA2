#include "MatrixOpCut.h"

MatrixOpCut::MatrixOpCut() : MatrixOpFiveArgs(3, "CUT") {}

Matrix MatrixOpCut::evaluate(const std::vector<Matrix> & args) const {
    const Matrix & offset_column = args[0];
    const Matrix & offset_row = args[1];
    const Matrix & new_columns = args[2];
    const Matrix & new_rows = args[3];
    Matrix  rhs = args[4];
    return rhs.cut(new_rows, new_columns, offset_row, offset_column);
}
