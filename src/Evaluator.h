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
#include "ParsedInput.h"

class Evaluator : public InputHandler {
    using VariableMap = std::unordered_map<std::string, Matrix>;
  public:
    Evaluator(MatrixFactory, std::ostream &);
    void evaluate_input(const ParsedInput &);

  private:
    std::ostream & _stream;
    VariableMap _vars;

    struct ContainerOperations {
        ContainerOperations(
            const std::function<Matrix(const std::string &)> & getter,
            const std::function<void(const std::string &, const Matrix &)> &
                replacer)
            : get_var(getter), replace_var(replacer) {}
        std::function<Matrix(const std::string &)> get_var;
        std::function<void(const std::string &, const Matrix &)> replace_var;
    };

    void handle_one_arg(std::stack<std::string> &, Operator, const ContainerOperations &);
    void handle_two_args(std::stack<std::string> &, Operator, const ContainerOperations &);
    void handle_five_args(std::stack<std::string> &, Operator, const ContainerOperations &);
    void load_nontmp_vars(const VariableMap &);
};
