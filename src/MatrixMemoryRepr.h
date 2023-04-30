#pragma once

#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>
#include "MatrixDimensions.h"
#include "IteratorWrapper.h"

class MatrixMemoryRepr {
  public:
    MatrixMemoryRepr() = delete;
    MatrixMemoryRepr(std::size_t, std::size_t);
    virtual ~MatrixMemoryRepr() = default;
    virtual MatrixMemoryRepr * clone() const = 0;
    friend std::ostream & operator<<(std::ostream &, const MatrixMemoryRepr &);
    std::size_t rows() const;
    std::size_t columns() const;
    virtual std::optional<double> at(std::size_t, std::size_t) const = 0;
    virtual void add(std::size_t, std::size_t, double) = 0;
    virtual void modify(std::size_t, std::size_t, double) = 0;
    virtual void swap_rows(std::size_t, std::size_t) = 0;
    virtual bool is_efficient(double) const = 0;

    virtual IteratorWrapper begin() const = 0;
    virtual IteratorWrapper end() const = 0;

  protected:
    MatrixDimensions _dimensions;
    virtual void print(std::ostream &) const = 0;
};