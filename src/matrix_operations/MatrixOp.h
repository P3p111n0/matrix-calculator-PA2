#pragma once

#include "../matrix_wrapper/Matrix.h"
#include <cstdlib>
#include <string>

class MatrixOp {
  public:
    MatrixOp(std::size_t, std::size_t, const std::string &);
    virtual ~MatrixOp() = default;
    std::size_t arity() const;
    std::size_t priority() const;
    const std::string & name() const;
    virtual Matrix evaluate(const std::vector<Matrix> &) const = 0;
  protected:
    std::size_t _arity;
    std::size_t _priority;
    std::string _name;
};
