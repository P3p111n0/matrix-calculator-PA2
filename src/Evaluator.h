#pragma once

#include "Matrix.h"
#include "MatrixFactory.h"
#include "Operator.h"
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>

class Evaluator {
    using OutputQueue = std::shared_ptr<std::queue<std::string>>;
    using VariableMap = std::unordered_map<std::string, Matrix>;

  public:
    Evaluator(MatrixFactory);
    bool evaluate_input(VariableMap &, OutputQueue &) const;

  private:
    MatrixFactory _factory;

    void handle_one_arg(std::stack<std::string> &, VariableMap &,
                        Operator) const;
    void handle_two_args(std::stack<std::string> &, VariableMap &,
                         Operator) const;
    void handle_five_args(std::stack<std::string> &, VariableMap &,
                          Operator) const;
};
