#include "Matrix.h"

Rational Matrix::value_ratio = Rational(2, 3);

Matrix::Matrix(std::size_t, std::size_t) {}

Matrix::Matrix(std::initializer_list<std::initializer_list<Rational>>) {}

Matrix::Matrix(const Matrix &) {}

Matrix::Matrix(Matrix &&) noexcept {}

Matrix & Matrix::operator=(const Matrix &) { return *this; }

Matrix & Matrix::operator=(Matrix &&) noexcept { return *this; }

Matrix Matrix::operator+(const Matrix &) const { return Matrix(0, 0); }

Matrix Matrix::operator-(const Matrix &) const { return Matrix(0, 0); }

Matrix Matrix::operator*(const Matrix &) const { return Matrix(0, 0); }

Matrix operator*(const Rational &, const Matrix &) { return Matrix(0, 0); }

std::size_t Matrix::rows() const {
    return _matrix->rows();
}

std::size_t Matrix::columns() const {
    return _matrix->columns();
}

IteratorWrapper Matrix::begin() const { return _matrix->begin(); }

IteratorWrapper Matrix::end() const { return _matrix->end(); }

void Matrix::transpose() {}

void Matrix::unite(const Matrix &) {}

void Matrix::cut(std::size_t) {}

void Matrix::inverse() {}

std::optional<Rational> Matrix::det() const {
    return std::nullopt;
}

std::optional<std::size_t> Matrix::rank() const {
    return std::nullopt;
}

void Matrix::gem() {}

std::ostream & operator<<(std::ostream & os, const Matrix & mx) {
    os << *(mx._matrix);
    return os;
}
