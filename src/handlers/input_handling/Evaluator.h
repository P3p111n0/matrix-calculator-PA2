#pragma once

#include "../../matrix_operations/Operator.h"
#include "../../matrix_wrapper/Matrix.h"
#include "../../matrix_wrapper/MatrixFactory.h"
#include "ContainerOperations.h"
#include "InputHandler.h"
#include "ParsedInput.h"
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>

class Evaluator : public InputHandler {
    using VariableMap = std::unordered_map<std::string, Matrix>;
  public:
    Evaluator(MatrixFactory, std::ostream &);
    void evaluate_input(const ParsedInput &);

  private:
    std::ostream & _stream;
    VariableMap _vars;

    void handle_one_arg(std::stack<std::string> &, Operator, const ContainerOperations &);
    void handle_two_args(std::stack<std::string> &, Operator, const ContainerOperations &);
    void handle_five_args(std::stack<std::string> &, Operator, const ContainerOperations &);
    void load_nontmp_vars(const VariableMap &);
};
