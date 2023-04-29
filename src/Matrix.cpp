#include "Matrix.h"

Rational Matrix::value_ratio = Rational(2, 3);

Matrix::Matrix(std::size_t, std::size_t) {}

Matrix::Matrix(std::initializer_list<std::initializer_list<Rational>>) {}

Matrix::Matrix(const Matrix & src)
    : _matrix(src._matrix->clone()), _det(src._det), _rank(src._rank) {}

Matrix::Matrix(Matrix && src) noexcept
    : _matrix(std::move(src._matrix)), _det(src._det), _rank(src._rank) {}

Matrix & Matrix::operator=(const Matrix & src) {
    if (this != &src) {
        _matrix = std::unique_ptr<MatrixMemoryRepr>(src._matrix->clone());
        _det = src._det;
        _rank = src._rank;
    }
    return *this;
}

Matrix & Matrix::operator=(Matrix && src) noexcept {
    if (this != &src) {
        _matrix = std::move(src._matrix);
        _det = src._det;
        _rank = src._rank;
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix & other) const {
    if (rows() != other.rows() || columns() != other.columns()) {
        throw std::invalid_argument(
            "Matrix addition: dimensions are not matching.");
    }
    Matrix result(*this);
    for (const auto & element : other) {
        result._matrix->add(element.row, element.column, element.value);
    }
    return result;
}

Matrix Matrix::operator-(const Matrix & other) const {
    if (rows() != other.rows() || columns() != other.columns()) {
        throw std::invalid_argument(
            "Matrix subtraction: dimensions are not matching.");
    }
    Matrix result(*this);
    for (const auto & element : other) {
        result._matrix->add(element.row, element.column, element.value * -1);
    }
    return result;
}

Matrix Matrix::operator*(const Matrix & other) const {
    if (columns() != other.rows()) {
        throw std::invalid_argument(
            "Matrix multiplication: invalid matrix dimensions.");
    }
    Matrix result(rows(), other.columns());

    for (std::size_t i = 0; i < result.rows(); i++) {
        for (std::size_t j = 0; j < result.columns(); j++) {
            Rational result_element = 0;
            for (std::size_t k = 0; k < columns(); k++) {
                result_element +=
                    _matrix->at(i, k).value() * other._matrix->at(k, j).value();
            }
            result._matrix->modify(i, j, result_element);
        }
    }

    return result;
}

Matrix operator*(const Rational & scalar, const Matrix & mx) {
    Matrix result(mx);
    for (const auto & element : mx) {
        Rational new_value = element.value * scalar;
        result._matrix->modify(element.row, element.column, new_value);
    }
    return result;
}

std::size_t Matrix::rows() const { return _matrix->rows(); }

std::size_t Matrix::columns() const { return _matrix->columns(); }

IteratorWrapper Matrix::begin() const { return _matrix->begin(); }

IteratorWrapper Matrix::end() const { return _matrix->end(); }

void Matrix::transpose() {
    Matrix transposed(columns(), rows());
    for (const auto & [row, col, val] : *this) {
        transposed._matrix->modify(col, row, val);
    }
    _matrix = std::move(transposed._matrix);
}

void Matrix::unite(const Matrix & other) {
    if (columns() != other.columns()) {
        throw std::invalid_argument("Unite: matrix dimension mismatch");
    }
    Matrix united(rows() + other.rows(), columns());
    for (const auto & [row, col, val] : *this) {
        united._matrix->modify(row, col, val);
    }
    for (const auto & [row, col, val] : other) {
        united._matrix->modify(row + rows(), col, val);
    }
    _matrix = std::move(united._matrix);
}

void Matrix::cut(std::size_t) {}

void Matrix::inverse() {}

std::optional<Rational> Matrix::det() const { return std::nullopt; }

std::optional<std::size_t> Matrix::rank() const { return std::nullopt; }

void Matrix::gem() {}

std::ostream & operator<<(std::ostream & os, const Matrix & mx) {
    os << *(mx._matrix);
    return os;
}

void Matrix::gem_swap_rows(
    std::function<void(std::size_t, std::size_t)> && capture_fn) {

    for (std::size_t i = 0, column_index = 0;
         i < rows() && column_index < columns(); i++, column_index++) {
        if (_matrix->at(i, column_index).value() == 0) {
            for (std::size_t j = i; j < rows(); j++) {
                for (std::size_t h = column_index; h < columns(); h++) {
                    if (_matrix->at(i, h).value() == 0 && _matrix->at(j, h).value() != 0) {
                        _matrix->swap_rows(i, j);
                        capture_fn(i, j);
                        break;
                    }
                }
            }
        }
    }
}


// https://www.math-cs.gordon.edu/courses/ma342/handouts/gauss.pdf
void Matrix::gem_row_elim(
    std::function<void(std::size_t, std::size_t)> && capture_fn) {

    for (std::size_t i = 0; i < rows() - 1; i++) {
        for (std::size_t j = i + 1; j < rows(); j++) {
            capture_fn(i, j);
            Rational multiplier = _matrix->at(j, i).value();
            for (std::size_t k = i; k < columns(); k++) {
                _matrix->modify(
                    j, k,
                    (_matrix->at(j, k).value() * _matrix->at(i, i).value() -
                     (multiplier * _matrix->at(i, k).value())));
            }
        }
    }
}