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
    if (rows() != other.rows() || columns() != other.columns()){
        throw std::invalid_argument("Matrix addition: dimensions are not matching.");
    }
    Matrix result(*this);
    for (const auto & element : other){
        result._matrix->add(element.row, element.column, element.value);
    }
    return result;
}

Matrix Matrix::operator-(const Matrix & other) const {
    if (rows() != other.rows() || columns() != other.columns()){
        throw std::invalid_argument("Matrix subtraction: dimensions are not matching.");
    }
    Matrix result(*this);
    for (const auto & element : other){
        result._matrix->add(element.row, element.column, element.value * -1);
    }
    return result;
}

Matrix Matrix::operator*(const Matrix &) const { return Matrix(0, 0); }

Matrix operator*(const Rational & scalar, const Matrix & mx) {
    Matrix result(mx);
    for (const auto & element : mx){
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
    for (const auto & [row, col, val] : *this){
        transposed._matrix->modify(col, row, val);
    }
    _matrix = std::move(transposed._matrix);
}

void Matrix::unite(const Matrix & other) {
    if (columns() != other.columns()){
        throw std::invalid_argument("Unite: matrix dimension mismatch");
    }
    Matrix united(rows() + other.rows(), columns());
    for (const auto & [row, col, val] : *this){
        united._matrix->modify(row, col, val);
    }
    for (const auto & [row, col, val] : other){
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
