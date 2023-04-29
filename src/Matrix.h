#pragma once

#include "MatrixMemoryRepr.h"
#include "Rational.h"
#include <initializer_list>
#include <memory>
#include <optional>
#include <iostream>
#include <functional>

class Matrix {
  public:
    Matrix(std::size_t, std::size_t);
    Matrix(std::initializer_list<std::initializer_list<Rational>>);
    Matrix(const Matrix &);
    Matrix(Matrix &&) noexcept;

    Matrix & operator=(const Matrix &);
    Matrix & operator=(Matrix &&) noexcept;

    Matrix operator+(const Matrix &) const;
    Matrix operator-(const Matrix &) const;
    Matrix operator*(const Matrix &) const;
    friend Matrix operator*(const Rational &, const Matrix &);

    IteratorWrapper begin() const;
    IteratorWrapper end() const;

    std::size_t rows() const;
    std::size_t columns() const;

    void transpose();
    void unite(const Matrix &);
    void cut(std::size_t);
    void inverse();
    std::optional<Rational> det();
    std::optional<Rational> det() const;
    std::size_t rank();
    std::size_t rank() const;
    void gem();

    friend std::ostream & operator<<(std::ostream &, const Matrix &);

  private:
    std::unique_ptr<MatrixMemoryRepr> _matrix;
    std::optional<Rational> _det;
    std::optional<std::size_t> _rank;

    void gem_swap_rows(std::function<void(std::size_t, std::size_t)> &&);
    void gem_row_elim(std::function<void(std::size_t, std::size_t)> &&);

    std::optional<Rational> calc_det() const;
    std::size_t calc_rank() const;

    static Rational value_ratio;
};