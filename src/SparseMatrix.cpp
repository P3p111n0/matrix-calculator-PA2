#include "SparseMatrix.h"
#include <algorithm>
#include <memory>

bool SparseMatrix::MatrixElementComparator::operator()(
    const MatrixElement & lhs, const MatrixElement & rhs) const {
    if (lhs.row != rhs.row) {
        return lhs.row < rhs.row;
    }
    return lhs.column < rhs.column;
}

SparseMatrix::SparseMatrix(size_t rows, size_t columns)
    : MatrixMemoryRepr(rows, columns) {}

SparseMatrix::SparseMatrix(const std::vector<MatrixElement> & dump)
    : MatrixMemoryRepr(dump.back().row + 1, dump.back().column + 1) {

    std::copy_if(dump.begin(), dump.end(), std::inserter(_data, _data.begin()),
                 [](const MatrixElement & e) { return e.value != 0; });
}

SparseMatrix::SparseMatrix(
    std::initializer_list<std::initializer_list<int>> init_list)
    : MatrixMemoryRepr(init_list.size(),
                       init_list.size() ? init_list.begin()->size() : 0) {

    std::size_t row = 0;
    for (const auto & list : init_list) {
        std::size_t col = 0;
        for (const auto & val : list) {
            if (val != 0) {
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
            "Determinant is undefined for non-square matrices");
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
            "Determinant is undefined for non-square matrices");
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
    auto matrix_array = dump();
    std::vector<std::vector<int>> matrix;

    for (std::size_t i = 0; i < _rows; i++) {
        std::vector<int> tmp;
        matrix.emplace_back(tmp);
    }

    for (const auto & i : matrix_array) {
        matrix[i.row].emplace_back(i.value);
    }

    for (std::size_t i = 0, column_index = 0;
         i < _rows && column_index < _columns; i++, column_index++) {
        if (matrix[i][column_index] == 0) {
            for (std::size_t j = i; j < _rows; j++) {
                for (std::size_t h = column_index; h < _columns; h++) {
                    if (matrix[i][h] == 0 && matrix[j][h] != 0) {
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

    std::vector<MatrixElement> gaussed_matrix_dump;
    for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _columns; j++) {
            gaussed_matrix_dump.emplace_back(i, j, matrix[i][j]);
        }
    }

    std::set<MatrixElement, MatrixElementComparator> new_data;
    std::copy_if(gaussed_matrix_dump.begin(), gaussed_matrix_dump.end(),
                 std::inserter(new_data, new_data.begin()),
                 [](const MatrixElement & e) { return e.value != 0; });

    _data = std::move(new_data);
}

void SparseMatrix::inverse() {}

void SparseMatrix::transpose() {
    std::set<MatrixElement, MatrixElementComparator> new_data;
    for (const auto & i : _data) {
        new_data.emplace(i.column, i.row, i.value);
    }
    _data = std::move(new_data);
}

void SparseMatrix::unite(const MatrixMemoryRepr & other) {
    auto other_dump = other.dump();

    for (auto & element : other_dump) {
        if (element.value != 0) {
            element.row += _rows;
            element.column += _columns;
        }
    }
    _rows += other.rows();
    _columns += other.columns();

    std::copy_if(other_dump.begin(), other_dump.end(),
                 std::inserter(_data, _data.begin()),
                 [](const MatrixElement & e) { return e.value != 0; });
}

std::vector<MatrixElement> SparseMatrix::dump() const {
    auto intermediate = _data;
    std::vector<MatrixElement> result;
    for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _columns; j++) {
            intermediate.emplace(i, j, 0);
        }
    }
    copy(intermediate.begin(), intermediate.end(), std::back_inserter(result));
    return result;
}

void SparseMatrix::print(std::ostream & os) const {
    auto self_dump = dump();
    for (std::size_t i = 0; i < _rows * _columns; i += _columns) {
        os << "[ ";
        for (std::size_t j = 0; j < _columns - 1; j++) {
            os << self_dump[i + j].value << ", ";
        }
        os << self_dump[i + _columns - 1].value << " ]";
        if (i + _columns - 1 != self_dump.size() - 1){
            os << std::endl;
        }
    }
}

double SparseMatrix::calc_det() const {
    std::unique_ptr<MatrixMemoryRepr> matrix_copy(clone());
    auto matrix_array = matrix_copy->dump();
    std::vector<std::vector<int>> matrix;

    std::vector<int> row;
    size_t last_row = matrix_array.front().row;

    for (const auto & element : matrix_array) {
        if (last_row != element.row) {
            matrix.emplace_back(row);
            row.clear();
            last_row = element.row;
        }
        row.emplace_back(element.value);
    }
    matrix.emplace_back(row);

    std::vector<int> division_vec;
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

    for (std::size_t i = 0; i < _rows * _columns; i += _columns) {
        for (std::size_t j = 0; j < _columns; j++) {
            if (copy_dump[i + j].value != 0) {
                rank++;
                break;
            }
        }
    }

    return rank;
}
