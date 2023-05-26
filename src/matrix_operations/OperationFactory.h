#pragma once

#include "MatrixOp.h"
#include <memory>
#include <set>
#include <unordered_map>

class OperationFactory {
  public:
    OperationFactory();
    std::shared_ptr<MatrixOp> get_operation(const std::string &) const;
    bool is_operation(const std::string &) const;
    std::size_t priority_of(const std::string &) const;
  private:
    std::unordered_map<std::string, std::shared_ptr<MatrixOp>> _operations;
};
