#pragma once

#include "MatrixOp.h"
#include "Operator.h"
#include <memory>
#include <set>
#include <unordered_map>

struct OperationCmp {
    bool operator()(const std::shared_ptr<MatrixOp> &,
                           const std::shared_ptr<MatrixOp> &) const;
};

class OperationFactory {
  public:
    OperationFactory();
    MatrixOp * get_operation(const std::string &) const;
    std::set<std::shared_ptr<MatrixOp>, OperationCmp> get_all_operations() const;
    bool is_operation(const std::string &) const;
  private:
    std::unordered_map<std::string, Operator> _operations;
};
