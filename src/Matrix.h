#pragma once

#include "MatrixMemoryRepr.h"
#include "MatrixFactory.h"
#include <initializer_list>
#include <memory>
#include <optional>
#include <iostream>
#include <functional>

class Matrix {
  public:
    Matrix(std::size_t, std::size_t);
    Matrix(std::initializer_list<std::initializer_list<double>>);
    Matrix(const Matrix &);
    Matrix(Matrix &&) noexcept;

    Matrix & operator=(const Matrix &);
    Matrix & operator=(Matrix &&) noexcept;

    Matrix operator+(const Matrix &) const;
    Matrix operator-(const Matrix &) const;
    Matrix operator*(const Matrix &) const;
    friend Matrix operator*(double, const Matrix &);

    IteratorWrapper begin() const;
    IteratorWrapper end() const;

    std::size_t rows() const;
    std::size_t columns() const;

    void transpose();
    void unite(const Matrix &);
    void cut(std::size_t, std::size_t, std::size_t, std::size_t);
    void inverse();
    std::optional<double> det();
    std::optional<double> det() const;
    std::size_t rank();
    std::size_t rank() const;
    void gem();

    friend std::ostream & operator<<(std::ostream &, const Matrix &);

  private:
    std::unique_ptr<MatrixMemoryRepr> _matrix;
    std::optional<double> _det;
    std::optional<std::size_t> _rank;
    MatrixFactory _factory;

    void gem_swap_rows(std::function<void(std::size_t, std::size_t)> &&);
    void gem_row_elim(std::function<void(std::size_t, std::size_t)> &&);
    void optimize();

    std::optional<double> calc_det() const;
    std::size_t calc_rank() const;
};