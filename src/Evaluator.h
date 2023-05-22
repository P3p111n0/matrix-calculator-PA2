#pragma once

#include <memory>
#include <queue>
#include <string>
#include <stack>
#include <unordered_map>
#include "InputHandler.h"
#include "Matrix.h"
#include "MatrixFactory.h"
#include "Operator.h"

class Evaluator : public InputHandler {
    using OutputQueue = std::shared_ptr<std::queue<std::string>>;
    using VariableMap = std::unordered_map<std::string, Matrix>;

  public:
    Evaluator(MatrixFactory, std::ostream &);
    bool evaluate_input(VariableMap &, OutputQueue &) const;

  private:
    std::ostream & _stream;

    void handle_one_arg(std::stack<std::string> &, VariableMap &,
                        Operator) const;
    void handle_two_args(std::stack<std::string> &, VariableMap &,
                         Operator) const;
    void handle_five_args(std::stack<std::string> &, VariableMap &,
                          Operator) const;
    static void cleanup(VariableMap &);
};
