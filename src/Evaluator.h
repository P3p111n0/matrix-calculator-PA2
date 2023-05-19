#pragma once

#include "Matrix.h"
#include <memory>
#include <string>
#include <queue>
#include <unordered_map>

class Evaluator {
    using OutputQueue = std::shared_ptr<std::queue<std::string>>;
    using VariableMap = std::unordered_map<std::string, Matrix>;
  public:
    Evaluator() = default;
    bool evaluate_input(VariableMap &, OutputQueue &) const;
};
