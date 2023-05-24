#pragma once

#include "../../matrix_wrapper/Matrix.h"
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>

class ParsedInput {
    using OutputQueuePointer = std::shared_ptr<std::queue<std::string>>;
    using VariableMapPointer = std::shared_ptr<std::unordered_map<std::string, Matrix>>;
  public:
    ParsedInput();
    OutputQueuePointer output_queue;
    VariableMapPointer loaded_variables;
};
