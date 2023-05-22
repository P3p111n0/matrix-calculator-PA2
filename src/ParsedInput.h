#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <queue>
#include "Matrix.h"

class ParsedInput {
    using OutputQueuePointer = std::shared_ptr<std::queue<std::string>>;
    using VariableMapPointer = std::shared_ptr<std::unordered_map<std::string, Matrix>>;
  public:
    ParsedInput();
    OutputQueuePointer output_queue;
    VariableMapPointer loaded_variables;
};
