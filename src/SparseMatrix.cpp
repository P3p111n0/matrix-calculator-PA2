#include "SparseMatrix.h"
#include "MatrixElement.h"
#include "SparseMatrixIterator.h"

SparseMatrix::SparseMatrix(std::size_t r, std::size_t c)
    : MatrixMemoryRepr(r, c) {
    if (!_dimensions.rows() || !_dimensions.columns()){
        throw std::invalid_argument("Invalid matrix dimensions.");
    }

}

SparseMatrix::SparseMatrix(
    std::initializer_list<std::initializer_list<double>> init_list)
    : MatrixMemoryRepr(init_list.size(),
                       init_list.size() ? init_list.begin()->size() : 0) {
    if (!_dimensions.rows() || !_dimensions.columns()){
        throw std::invalid_argument("Invalid dimensions of an initializer list.");
    }

    std::size_t row = 0;
    for (const auto & list : init_list) {
        std::size_t col = 0;
        for (const auto & val : list) {
            if (val != 0) {
                _data[{row, col}] = val;
            }
            ++col;
        }
        if (col != _dimensions.columns()) {
            throw std::invalid_argument(
                "Column size mismatch in initializer list.");
        }
        ++row;
    }
}

SparseMatrix::SparseMatrix(IteratorWrapper begin, IteratorWrapper end)
    : MatrixMemoryRepr(begin.get_matrix_rows(), begin.get_matrix_columns()) {

    for (; begin != end; ++begin) {
        const auto & [pos, val] = *begin;
        _data[pos] = val;
    }
}

MatrixMemoryRepr * SparseMatrix::clone() const {
    return new SparseMatrix(*this);
}

std::optional<double> SparseMatrix::at(std::size_t row,
                                       std::size_t column) const {
    if (row >= _dimensions.rows() || column >= _dimensions.columns()) {
        return std::nullopt;
    }
    if (!_data.count({row, column})) {
        return 0;
    }
    return _data.at({row, column});
}

void SparseMatrix::add(std::size_t row, std::size_t column, double val) {
    if (row >= _dimensions.rows() || column >= _dimensions.columns()) {
        throw std::out_of_range("Add: index out of bounds");
    }
    _data[{row, column}] += val;
}

void SparseMatrix::modify(std::size_t row, std::size_t column, double new_val) {
    if (row >= _dimensions.rows() || column >= _dimensions.columns()) {
        throw std::out_of_range("Modify: index out of bounds");
    }
    if (new_val != 0) {
        _data[{row, column}] = new_val;
    }
}

void SparseMatrix::swap_rows(std::size_t f_row, std::size_t s_row) {
    std::vector<MatrixElement> swap_elements;
    if (f_row >= _dimensions.rows() || s_row >= _dimensions.columns()) {
        throw std::out_of_range("Swap_rows: index out of range");
    }
    for (const auto & [key, value] : _data) {
        if (key.row == f_row || key.row == s_row) {
            swap_elements.emplace_back(key.row, key.column, value);
        }
    }
    for (const auto & [pos, val] : swap_elements) {
        Position new_pos = {pos.row == f_row ? s_row : f_row, pos.column};
        _data[new_pos] = val;
    }
}

void SparseMatrix::print(std::ostream & os) const {
    for (std::size_t i = 0; i < _dimensions.rows(); i++) {
        os << "[ ";
        for (std::size_t j = 0; j < _dimensions.columns() - 1; j++) {
            auto elem = _data.find({i, j});
            double val = elem == _data.end()
                             ? 0
                             : (elem->second == 0 ? 0 : elem->second);
            os << val << ", ";
        }
        auto last_elem = _data.find({i, _dimensions.columns() - 1});
        double last_val =
            last_elem == _data.end()
                ? 0
                : (last_elem->second == 0 ? 0 : last_elem->second);
        os << last_val << " ]";
        if (i != _dimensions.rows() - 1) {
            os << std::endl;
        }
    }
}

IteratorWrapper SparseMatrix::begin() const {
    return {new SparseMatrixIterator(this, _data.begin())};
}

IteratorWrapper SparseMatrix::end() const {
    return {new SparseMatrixIterator(this, _data.end())};
}
bool SparseMatrix::is_efficient(double ratio) const {
    return _data.size() <=
           (1 - ratio) * (_dimensions.rows() * _dimensions.columns());
}