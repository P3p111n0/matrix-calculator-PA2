#include "MatrixOp.h"

MatrixOp::MatrixOp(std::size_t arity, std::size_t priority,
                   const std::string & name)
    : _arity(arity), _priority(priority), _name(name) {}

std::size_t MatrixOp::arity() const { return _arity; }

std::size_t MatrixOp::priority() const { return _priority; }

const std::string & MatrixOp::name() const { return _name; }