#include "SparseMatrix.h"
#include "Rational.h"
#include "MatrixElement.h"

bool SparseMatrix::Position::operator<(const SparseMatrix::Position & rhs) const {
    return row == rhs.row ? col < rhs.col : row < rhs.row;
}

SparseMatrix::SparseMatrix(std::size_t r, std::size_t c)
    : MatrixMemoryRepr(r, c) {}

SparseMatrix::SparseMatrix(
    std::initializer_list<std::initializer_list<Rational>> init_list)
    : MatrixMemoryRepr(init_list.size(),
                       init_list.size() ? init_list.begin()->size() : 0) {

    std::size_t row = 0;
    for (const auto & list : init_list) {
        std::size_t col = 0;
        for (const auto & val : list) {
            if (val != 0) {
                _data[{row, col}] = val;
            }
            ++col;
        }
        ++row;
    }
}

MatrixMemoryRepr * SparseMatrix::clone() const {
    return new SparseMatrix(*this);
}

Rational SparseMatrix::at(std::size_t row, std::size_t column) const {
    if (!_data.count({row, column})) {
        return 0;
    }
    return _data.at({row, column});
}

void SparseMatrix::add(std::size_t row, std::size_t column,
                       const Rational & val) {

    _data[{row, column}] += val;
}

void SparseMatrix::modify(std::size_t row, std::size_t column,
                          const Rational & new_val) {
    if (new_val != 0){
        _data[{row, column}] = new_val;
    }
}

void SparseMatrix::swap_rows(std::size_t f_row, std::size_t s_row) {
    std::vector<MatrixElement> swap_elements;

    for (const auto & [key, value] : _data) {
        if (key.row == f_row || key.row == s_row) {
            swap_elements.emplace_back(key.row, key.col, value);
        }
    }
    for (const auto & element : swap_elements) {
        Position new_pos = {element.row == f_row ? s_row : f_row, element.column};
        _data[new_pos] = element.value;
    }
}

void SparseMatrix::print(std::ostream & os) const {
    for (std::size_t i = 0; i < _rows; i++) {
        os << "[ ";
        for (std::size_t j = 0; j < _columns - 1; j++) {
            auto elem = _data.find({i, j});
            Rational val = elem == _data.end() ? 0 : elem->second;
            os << val << ", ";
        }
        auto last_elem = _data.find({i, _columns - 1});
        Rational last_val = last_elem == _data.end() ? 0 : last_elem->second;
        os << last_val << " ]";
        if (i != _rows - 1) {
            os << std::endl;
        }
    }
}
