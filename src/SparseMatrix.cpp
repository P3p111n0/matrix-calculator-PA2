#include "SparseMatrix.h"
#include "Rational.h"

SparseMatrix::SparseMatrix(std::size_t r, std::size_t c)
    : MatrixMemoryRepr(r, c) {}

SparseMatrix::SparseMatrix(const SparseMatrix::MemoryDump & mem)
    : MatrixMemoryRepr(mem.size(), mem.size() ? mem.at(0).size() : 0) {

    for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _columns; j++) {
            if (mem[i][j] != 0) {
                _data.emplace(i, j, mem[i][j]);
            }
        }
    }
}

SparseMatrix::SparseMatrix(
    std::initializer_list<std::initializer_list<Rational>> init_list)
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

Rational SparseMatrix::at(std::size_t row, std::size_t column) const {
    MatrixElement dummy(row, column, 0);
    if (!_data.count(dummy)) {
        return 0;
    }
    return _data.find(dummy)->value;
}

void SparseMatrix::add(std::size_t row, std::size_t column,
                       const Rational & val) {

    auto req_element = _data.find(MatrixElement(row, column, 0));
    if (req_element == _data.end()) {
        _data.emplace(row, column, val);
        return;
    }

    Rational found_val = req_element->value;
    _data.erase(req_element);
    found_val += val;
    _data.emplace(row, column, found_val);
}

void SparseMatrix::modify(std::size_t row, std::size_t column,
                          const Rational & new_val) {
    auto element_to_modify = _data.find(MatrixElement(row, column, 0));
    if (element_to_modify == _data.end()) {
        _data.emplace(row, column, new_val);
        return;
    }
    _data.erase(element_to_modify);
    _data.emplace(row, column, new_val);
}

void SparseMatrix::swap_rows(std::size_t f_row, std::size_t s_row) {
    std::vector<MatrixElement> swap_elements;

    for (const auto & i : _data) {
        if (i.row == f_row || i.row == s_row) {
            swap_elements.emplace_back(i);
        }
    }
    for (const auto & element : swap_elements) {
        auto erase_it = _data.find(element);
        _data.erase(erase_it);
        _data.emplace(element.row == f_row ? s_row : f_row, element.column,
                      element.value);
    }
}

std::vector<std::vector<Rational>> SparseMatrix::dump() const {
    std::vector<std::vector<Rational>> result;
    for (std::size_t i = 0; i < _rows; i++) {
        std::vector<Rational> row;
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
    for (std::size_t i = 0; i < _rows; i++) {
        os << "[ ";
        for (std::size_t j = 0; j < _columns - 1; j++) {
            auto elem = _data.find(MatrixElement(i, j, 0));
            Rational val = elem == _data.end() ? 0 : elem->value;
            os << val << ", ";
        }
        auto last_elem = _data.find(MatrixElement(i, _columns - 1, 0));
        Rational last_val = last_elem == _data.end() ? 0 : last_elem->value;
        os << last_val << " ]";
        if (i != _rows - 1) {
            os << std::endl;
        }
    }
}
