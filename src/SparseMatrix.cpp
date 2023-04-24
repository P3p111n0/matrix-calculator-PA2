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
    : MatrixMemoryRepr(dump.back().row, dump.back().column) {

    std::copy_if(dump.begin(), dump.end(), std::inserter(_data, _data.begin()),
                 [](const MatrixElement & e) { return e.value != 0; });
}

MatrixMemoryRepr * SparseMatrix::clone() const {
    return new SparseMatrix(*this);
}

int SparseMatrix::det() {
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

int SparseMatrix::det() const {
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
    for (std::size_t i = 1; i < _rows; i++) {
        for (std::size_t j = 0; j < _columns; j++) {
            int source_coef = matrix_array[i + j].value;
            int target_coef = matrix_array[j].value;
            matrix_array[i + j].value *= target_coef;
            matrix_array[i + j].value -= matrix_array[j].value * source_coef;
        }
    }

    std::vector<std::vector<MatrixElement>> matrix;
    for (std::size_t i = 0; i < _rows; i++) {
        std::vector<MatrixElement> tmp;
        tmp.emplace_back(0, 0, 0);
        tmp.erase(tmp.begin());
        matrix.emplace_back(tmp);
    }

    for (const auto & i : matrix_array) {
        matrix[i.row].emplace_back(i);
    }

    std::size_t col_index = 0;
    for (std::size_t i = 0; i < _rows; i++) {
        if (col_index == _columns)
            break;
        if (matrix[i][col_index].value == 0) {
            for (std::size_t j = i; j < _rows; j++) {
                if (matrix[j][col_index].value != 0) {
                    std::swap(matrix[i], matrix[j]);
                    break;
                }
            }
        }
        ++col_index;
    }

    std::vector<MatrixElement> gaussed_matrix_dump;
    for (const auto & i : matrix) {
        for (const auto & j : i) {
            gaussed_matrix_dump.emplace_back(j);
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
    for (std::size_t i = 0; i < _rows; i++){
        os << "[ ";
        for (std::size_t j = 0; j < _columns - 1; j++){
            os << self_dump[i +j].value << ", ";
        }
        os << self_dump[i + _columns - 1].value << " ]" << std::endl;
    }
}

int SparseMatrix::calc_det() const {}

std::size_t SparseMatrix::calc_rank() const {
    std::size_t rank = std::min(_rows, _columns);
    std::unique_ptr<MatrixMemoryRepr> matrix_copy(clone());
    matrix_copy->gem();
    auto copy_dump = matrix_copy->dump();

    for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _columns; j++) {
            if (copy_dump[i + j].value != 0) {
                rank--;
                break;
            }
        }
    }

    return rank;
}
