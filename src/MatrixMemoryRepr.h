#pragma once

#ifndef MELCRJOS_MATRIXMEMORYREPR_H
#define MELCRJOS_MATRIXMEMORYREPR_H

#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>
#include "Rational.h"

class MatrixMemoryRepr {
  public:
    MatrixMemoryRepr() = delete;
    MatrixMemoryRepr(std::size_t, std::size_t);
    virtual ~MatrixMemoryRepr() = default;
    virtual MatrixMemoryRepr * clone() const = 0;
    friend std::ostream & operator<<(std::ostream &, const MatrixMemoryRepr &);
    std::size_t rows() const;
    std::size_t columns() const;
    virtual std::optional<Rational> at(std::size_t, std::size_t) const = 0;
    virtual void add(std::size_t, std::size_t, const Rational &) = 0;
    virtual void modify(std::size_t, std::size_t, const Rational &) = 0;
    virtual void swap_rows(std::size_t, std::size_t) = 0;

  protected:
    std::size_t _rows;
    std::size_t _columns;
    virtual void print(std::ostream &) const = 0;
};

#endif // MELCRJOS_MATRIXMEMORYREPR_H
