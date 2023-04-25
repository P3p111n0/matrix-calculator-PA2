#ifndef MELCRJOS_MATRIXMEMORYREPR_H
#define MELCRJOS_MATRIXMEMORYREPR_H

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
    size_t rows() const;
    size_t columns() const;

    virtual double det() = 0;
    virtual double det() const = 0;
    virtual std::size_t rank() = 0;
    virtual std::size_t rank() const = 0;
    virtual void gem() = 0;
    virtual void inverse() = 0;
    virtual void transpose() = 0;
    virtual void unite(const MatrixMemoryRepr &) = 0;
    virtual const std::vector<std::vector<double>> dump() const = 0;

  protected:
    std::size_t _rows;
    std::size_t _columns;
    std::optional<double> _det;
    std::optional<std::size_t> _rank;
    virtual void print(std::ostream &) const = 0;
    static bool dbl_cmp(double lhs, double rhs);
};

#endif // MELCRJOS_MATRIXMEMORYREPR_H
