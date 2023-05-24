#include "ParsedInput.h"

ParsedInput::ParsedInput()
    : output_queue(std::make_shared<std::queue<std::string>>()),
      loaded_variables(
          std::make_shared<std::unordered_map<std::string, Matrix>>()) {}
