#include "DenseMatrix.h"
#include "IteratorWrapper.h"
#include "DenseMatrixIterator.h"
#include "MatrixDimensions.h"
#include "MatrixMemoryRepr.h"
#include <vector>

DenseMatrix::DenseMatrix(std::size_t row, std::size_t col)
    : MatrixMemoryRepr(row, col), _data(row) {
    for (auto & vec : _data) {
        vec.resize(col);
    }
}

DenseMatrix::DenseMatrix(
    std::initializer_list<std::initializer_list<double>> init)
    : MatrixMemoryRepr(init.size(), init.size() ? init.begin()->size() : 0),
      _data(init.size()) {

    std::size_t row = 0;
    for (const auto & list : init) {
        _data[row].resize(_dimensions.columns());
        std::size_t col = 0;
        for (const auto & val : list) {
            _data[row][col] = val;
            ++col;
        }
        if (col != _dimensions.columns()) {
            throw std::invalid_argument(
                "Column size mismatch in initializer list.");
        }
        ++row;
    }
}

DenseMatrix::DenseMatrix(IteratorWrapper begin, IteratorWrapper end)
    : MatrixMemoryRepr(begin.get_matrix_rows(), begin.get_matrix_columns()),
      _data(_dimensions.rows()) {

    for (auto & column : _data) {
        column.resize(_dimensions.columns());
    }

    for (; begin != end; ++begin) {
        const auto & [pos, val] = *begin;
        _data[pos.row][pos.column] = val;
    }
}

MatrixMemoryRepr * DenseMatrix::clone() const { return new DenseMatrix(*this); }

std::optional<double> DenseMatrix::at(std::size_t row,
                                      std::size_t column) const {
    if (row >= _dimensions.rows() || column >= _dimensions.columns()) {
        return std::nullopt;
    }
    return _data[row][column];
}

void DenseMatrix::add(std::size_t row, std::size_t column, double val) {
    if (row >= _dimensions.rows() || column >= _dimensions.columns()) {
        throw std::out_of_range("Add: index of out bounds");
    }
    _data[row][column] += val;
}

void DenseMatrix::modify(std::size_t row, std::size_t column, double new_val) {
    if (row >= _dimensions.rows() || column >= _dimensions.columns()) {
        throw std::out_of_range("Modify: index out of bounds");
    }
    _data[row][column] = new_val;
}

void DenseMatrix::swap_rows(std::size_t f_row, std::size_t s_row) {
    if (f_row >= _dimensions.rows() || s_row >= _dimensions.columns()) {
        throw std::out_of_range("Swap_rows: index out of range");
    }
    std::swap(_data[f_row], _data[s_row]);
}

void DenseMatrix::print(std::ostream & os) const {
    for (std::size_t row_index = 0; row_index < _dimensions.rows();
         row_index++) {
        os << "[ ";
        for (std::size_t val_index = 0; val_index < _dimensions.columns() - 1;
             val_index++) {
            double val = _data[row_index][val_index] == 0
                             ? 0
                             : _data[row_index][val_index];
            os << val << ", ";
        }
        double last_val = _data[row_index][_dimensions.columns() - 1] == 0
                              ? 0
                              : _data[row_index][_dimensions.columns() - 1];
        os << last_val << " ]";
        if (row_index != _dimensions.rows() - 1) {
            os << std::endl;
        }
    }
}

IteratorWrapper DenseMatrix::begin() const {
    return {new DenseMatrixIterator(this, 0, 0)};
}

IteratorWrapper DenseMatrix::end() const {
    return {new DenseMatrixIterator(this, _dimensions.rows(), 0)};
}
bool DenseMatrix::is_efficient(double ratio) const {
    return begin().distance(end()) >
           (1 - ratio) * (_dimensions.rows() * _dimensions.columns());
}