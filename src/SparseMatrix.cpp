#include "SparseMatrix.h"

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

    for (const auto & element : dump) {
        if (element.value != 0) {
            _data.emplace(element);
        }
    }
}

MatrixMemoryRepr * SparseMatrix::clone() const {
    return new SparseMatrix(*this);
}

int SparseMatrix::det() {
    if (_rows != _columns){
        throw std::logic_error("Determinant is undefined for non-square matrices");
    }
    if (_det.has_value()){
        return _det.value();
    }
    _det = calc_det();
    return _det.value();
}

int SparseMatrix::det() const {
    if (_rows != _columns){
        throw std::logic_error("Determinant is undefined for non-square matrices");
    }
    if (_det.has_value()){
        return _det.value();
    }
    return calc_det();
}

int SparseMatrix::rank() {
    if (_rank.has_value()){
        return _rank.value();
    }
    _rank = calc_rank();
    return _rank.value();
}

int SparseMatrix::rank() const {
    if (_rank.has_value()){
        return _rank.value();
    }
    return calc_rank();
}

void SparseMatrix::gem() {}

void SparseMatrix::inverse() {}

void SparseMatrix::transpose() {
    std::set<MatrixElement, MatrixElementComparator> new_data;
    for (const auto & i : _data){
        new_data.emplace(i.column, i.row, i.value);
    }
    _data = std::move(new_data);
}

void SparseMatrix::unite(const MatrixMemoryRepr &) {}

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

void SparseMatrix::print(std::ostream &) const {}

int SparseMatrix::calc_det() const {}

int SparseMatrix::calc_rank() const {}
