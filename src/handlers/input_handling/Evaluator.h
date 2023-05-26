#pragma once

#include "../../matrix_wrapper/Matrix.h"
#include "../../matrix_wrapper/MatrixFactory.h"
#include "../file_handling/Exporter.h"
#include "../file_handling/Importer.h"
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
    Exporter _exporter;
    Importer _importer;
    void load_nontmp_vars(const VariableMap &);
};
