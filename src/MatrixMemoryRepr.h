#ifndef MELCRJOS_MATRIXMEMORYREPR_H
#define MELCRJOS_MATRIXMEMORYREPR_H

#include "MatrixElement.h"
#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>

class MatrixMemoryRepr {
  public:
    MatrixMemoryRepr() = delete;
    MatrixMemoryRepr(std::size_t, std::size_t);
    virtual ~MatrixMemoryRepr() = default;
    virtual MatrixMemoryRepr * clone() const = 0;
    friend std::ostream & operator<<(std::ostream &, const MatrixMemoryRepr &);

    virtual int det() = 0;
    virtual int det() const = 0;
    virtual int rank() = 0;
    virtual int rank() const = 0;
    virtual void gem() = 0;
    virtual MatrixMemoryRepr * gem() const = 0;
    virtual void inverse() = 0;
    virtual MatrixMemoryRepr * inverse() const = 0;
    virtual void transpose() = 0;
    virtual MatrixMemoryRepr * transpose() const = 0;
    virtual void unite(const MatrixMemoryRepr &) = 0;
    virtual MatrixMemoryRepr * unite(const MatrixMemoryRepr &) const = 0;
    virtual std::vector<MatrixElement> dump() const = 0;

  protected:
    std::size_t _rows;
    std::size_t _columns;
    std::optional<int> _det;
    std::optional<int> _rank;
    virtual void print(std::ostream &) const = 0;
};

#endif // MELCRJOS_MATRIXMEMORYREPR_H
