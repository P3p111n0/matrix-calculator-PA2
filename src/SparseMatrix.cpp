#include "SparseMatrix.h"
#include <algorithm>
#include <memory>

bool SparseMatrix::MatrixElement::operator<(
    const SparseMatrix::MatrixElement & rhs) const {
    if (row != rhs.row) {
        return row < rhs.row;
    }
    return column < rhs.column;
}

SparseMatrix::SparseMatrix(size_t rows, size_t columns)
    : MatrixMemoryRepr(rows, columns) {}

SparseMatrix::SparseMatrix(const MemoryDump & dump)
    : MatrixMemoryRepr(dump.size(), dump.size() ? dump.at(0).size() : 0) {

    memory_dump_to_data(dump);
}

SparseMatrix::SparseMatrix(
    std::initializer_list<std::initializer_list<int>> init_list)
    : MatrixMemoryRepr(init_list.size(),
                       init_list.size() ? init_list.begin()->size() : 0) {

    std::size_t row = 0;
    for (const auto & list : init_list) {
        std::size_t col = 0;
        for (const auto & val : list) {
            if (!dbl_eq(val, 0)) {
                _data.emplace(row, col, val);
            }
            ++col;
        }
        ++row;
    }
}

MatrixMemoryRepr * SparseMatrix::clone() const {
    return new SparseMatrix(*this);
}

double SparseMatrix::det() {
    if (_rows != _columns) {
        throw std::logic_error(
            "Determinant is undefined for non-square matrices.");
    }
    if (_det.has_value()) {
        return _det.value();
    }
    _det = calc_det();
    return _det.value();
}

double SparseMatrix::det() const {
    if (_rows != _columns) {
        throw std::logic_error(
            "Determinant is undefined for non-square matrices.");
    }
    if (_det.has_value()) {
        return _det.value();
    }
    return calc_det();
}

std::size_t SparseMatrix::rank() {
    if (_rank.has_value()) {
        return _rank.value();
    }
    _rank = calc_rank();
    return _rank.value();
}

std::size_t SparseMatrix::rank() const {
    if (_rank.has_value()) {
        return _rank.value();
    }
    return calc_rank();
}

void SparseMatrix::gem() {
    auto matrix = dump();

    for (std::size_t i = 0, column_index = 0;
         i < _rows && column_index < _columns; i++, column_index++) {
        if (dbl_eq(matrix[i][column_index], 0)) {
            for (std::size_t j = i; j < _rows; j++) {
                for (std::size_t h = column_index; h < _columns; h++) {
                    if (dbl_eq(matrix[i][h], 0) && !dbl_eq(matrix[j][h], 0)) {
                        std::swap(matrix[i], matrix[j]);
                        break;
                    }
                }
            }
        }
    }

    // https://www.math-cs.gordon.edu/courses/ma342/handouts/gauss.pdf
    for (std::size_t i = 0; i < _rows - 1; i++) {
        for (std::size_t j = i + 1; j < _rows; j++) {
            int multiplier = matrix[j][i];
            for (std::size_t k = i; k < _columns; k++) {
                matrix[j][k] =
                    (matrix[j][k] * matrix[i][i]) - (multiplier * matrix[i][k]);
            }
        }
    }

    memory_dump_to_data(matrix);
    _det.reset(); // invalidate determinant
}

// https://www.researchgate.net/publication/220337322_An_Efficient_and_Simple_Algorithm_for_Matrix_Inversion
void SparseMatrix::inverse() {
    if (_rows != _columns) {
        throw std::logic_error("Non-square matrices cannot be inverted.");
    }
    if (_det.has_value() && dbl_eq(_det.value(), 0)) {
        throw std::logic_error("Matrix is not invertible.");
    }

    double determinant = 1;
    std::size_t dim = _rows;
    auto matrix = dump();
    for (std::size_t pivot_loc = 0; pivot_loc < dim; pivot_loc++) {
        double pivot = matrix[pivot_loc][pivot_loc];
        determinant *= pivot; // step 3
        if (dbl_eq(pivot, 0)) {
            _det = 0;
            throw std::logic_error("Matrix is not invertible.");
        }
        // step 5
        for (std::size_t pivot_row = 0; pivot_row < dim; pivot_row++) {
            if (pivot_row != pivot_loc) {
                matrix[pivot_loc][pivot_row] /= pivot;
            }
        }
        // step 6
        for (std::size_t pivot_column = 0; pivot_column < dim; pivot_column++) {
            if (pivot_column != pivot_loc) {
                matrix[pivot_column][pivot_loc] /= (-1) * pivot;
            }
        }
        // step 7
        for (std::size_t j = 0; j < dim; j++) {
            if (pivot_loc != j) {
                for (std::size_t k = 0; k < dim; k++) {
                    if (pivot_loc != k) {
                        matrix[j][k] += matrix[pivot_loc][k] * matrix[j][k];
                    }
                }
            }
        }
        matrix[pivot_loc][pivot_loc] = 1 / pivot; // step 8
    }

    _det = determinant;
    memory_dump_to_data(matrix);
}

void SparseMatrix::transpose() {
    std::set<MatrixElement> new_data;
    for (const auto & i : _data) {
        new_data.emplace(i.column, i.row, i.value);
    }
    _data = std::move(new_data);
}

void SparseMatrix::unite(const MatrixMemoryRepr & other) {
    if (_rows != other.rows() || _columns != other.columns()) {
        throw std::logic_error("Matrices don't have the same dimensions.");
    }

    auto other_dump = other.dump();

    for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _columns; j++) {
            if (!dbl_eq(other_dump[i][j], 0)) {
                _data.emplace(i, j + _columns, other_dump[i][j]);
            }
        }
    }

    _columns += other.columns();
    _rank.reset(); // invalidate rank
}

std::vector<std::vector<double>> SparseMatrix::dump() const {
    std::vector<std::vector<double>> result;
    for (std::size_t i = 0; i < _rows; i++) {
        std::vector<double> row;
        for (std::size_t j = 0; j < _columns; j++) {
            row.emplace_back(0);
        }
        result.emplace_back(row);
    }
    for (const auto & e : _data) {
        result[e.row][e.column] = e.value;
    }

    return result;
}

void SparseMatrix::print(std::ostream & os) const {
    auto self_dump = dump();
    for (std::size_t i = 0; i < _rows; i++) {
        os << "[ ";
        for (std::size_t j = 0; j < _columns - 1; j++) {
            os << self_dump[i][j] << ", ";
        }
        os << self_dump[i][_columns - 1] << " ]";
        if (i != self_dump.size() - 1) {
            os << std::endl;
        }
    }
}

double SparseMatrix::calc_det() const {
    std::unique_ptr<MatrixMemoryRepr> matrix_copy(clone());
    auto matrix = matrix_copy->dump();

    std::vector<int> division_vec;
    for (std::size_t i = 0, column_index = 0;
         i < _rows && column_index < _columns; i++, column_index++) {
        if (dbl_eq(matrix[i][column_index], 0)) {
            for (std::size_t j = i; j < _rows; j++) {
                for (std::size_t h = column_index; h < _columns; h++) {
                    if (dbl_eq(matrix[i][h], 0) && !dbl_eq(matrix[j][h], 0)) {
                        std::swap(matrix[i], matrix[j]);
                        division_vec.emplace_back(-1);
                        break;
                    }
                }
            }
        }
    }

    // https://www.math-cs.gordon.edu/courses/ma342/handouts/gauss.pdf
    for (std::size_t i = 0; i < _rows - 1; i++) {
        for (std::size_t j = i + 1; j < _rows; j++) {
            division_vec.emplace_back(matrix[i][i]);
            int multiplier = matrix[j][i];
            for (std::size_t k = i; k < _columns; k++) {
                matrix[j][k] =
                    (matrix[j][k] * matrix[i][i]) - (multiplier * matrix[i][k]);
            }
        }
    }

    double det = 1;
    for (std::size_t i = 0; i < _rows; i++) {
        det *= matrix[i][i];
    }

    for (const auto & i : division_vec) {
        det /= i;
    }

    return det;
}

std::size_t SparseMatrix::calc_rank() const {
    std::size_t rank = 0;
    std::unique_ptr<MatrixMemoryRepr> matrix_copy(clone());
    matrix_copy->gem();
    auto copy_dump = matrix_copy->dump();

    for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _columns; j++) {
            if (dbl_eq(copy_dump[i][j], 0)) {
                rank++;
                break;
            }
        }
    }

    return rank;
}
void SparseMatrix::memory_dump_to_data(
    const SparseMatrix::MemoryDump & matrix) {
    _data.clear();
    for (std::size_t i = 0; i < matrix.size(); i++) {
        for (std::size_t j = 0; j < matrix[i].size(); j++) {
            if (!dbl_eq(matrix[i][j], 0)) {
                _data.emplace(i, j, matrix[i][j]);
            }
        }
    }
}
