#include "DenseMatrix.h"

class DenseMatrixIterator : public AbstractMatrixIterator {
  public:
    DenseMatrixIterator(std::size_t row, std::size_t column,
                        const DenseMatrix * data)
        : _row(row), _column(column), _ptr(data) {}
    void operator++() override {
        while (_ptr->_data[_row][_column] == 0 && _row < _ptr->_rows) {
            ++_column;
            _column %= _ptr->_columns;
        }
    }
    MatrixElement operator*() const override {
        return {_row, _column, _ptr->_data[_row][_column]};
    }
    bool operator==(const DenseMatrixIterator & other) const {
        return _row == other._row && _column == other._column &&
               _ptr == other._ptr;
    }
    bool operator!=(const DenseMatrixIterator & other) const {
        return !(*this == other);
    }

  private:
    std::size_t _row;
    std::size_t _column;
    const DenseMatrix * _ptr;
};

DenseMatrix::DenseMatrix(std::size_t row, std::size_t col)
    : MatrixMemoryRepr(row, col), _data(row) {
    for (auto & vec : _data) {
        vec.resize(col);
    }
}

DenseMatrix::DenseMatrix(
    std::initializer_list<std::initializer_list<Rational>> init)
    : MatrixMemoryRepr(init.size(), init.size() ? init.begin()->size() : 0) {

    std::size_t row = 0;
    for (const auto & list : init) {
        std::size_t col = 0;
        for (const auto & val : list) {
            _data[row][col] = val;
            ++col;
        }
        if (col != _columns) {
            throw std::invalid_argument(
                "Column size mismatch in initializer list.");
        }
        ++row;
    }
}

MatrixMemoryRepr * DenseMatrix::clone() const { return new DenseMatrix(*this); }

std::optional<Rational> DenseMatrix::at(std::size_t row,
                                        std::size_t column) const {
    if (row >= _rows || column >= _columns) {
        return std::nullopt;
    }
    return _data[row][column];
}

void DenseMatrix::add(std::size_t row, std::size_t column,
                      const Rational & val) {
    if (row >= _rows || column >= _columns) {
        throw std::out_of_range("Add: index of out bounds");
    }
    _data[row][column] += val;
}

void DenseMatrix::modify(std::size_t row, std::size_t column,
                         const Rational & new_val) {
    if (row >= _rows || column >= _columns) {
        throw std::out_of_range("Modify: index out of bounds");
    }
    _data[row][column] = new_val;
}

void DenseMatrix::swap_rows(std::size_t f_row, std::size_t s_row) {
    if (f_row >= _rows || s_row >= _rows) {
        throw std::out_of_range("Swap_rows: index out of range");
    }
    std::swap(_data[f_row], _data[s_row]);
}

void DenseMatrix::print(std::ostream & os) const {
    for (std::size_t row_index = 0; row_index < _rows; row_index++) {
        os << "[ ";
        for (std::size_t val_index = 0; val_index < _columns - 1; val_index++) {
            os << _data[row_index][val_index] << ", ";
        }
        os << _data[row_index][_columns - 1] << " ]";
        if (row_index != _rows - 1) {
            os << std::endl;
        }
    }
}

IteratorWrapper DenseMatrix::begin() const {
    return {new DenseMatrixIterator(0, 0, this)};
}

IteratorWrapper DenseMatrix::end() const {
    return {new DenseMatrixIterator{_rows, 0, this}};
}
