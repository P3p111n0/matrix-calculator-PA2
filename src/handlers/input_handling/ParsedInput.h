#pragma once

#include "../../matrix_wrapper/Matrix.h"
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>

/**
 * @brief Struct representing parsed input from an input stream. Assumes
 *        input parsed to Reverse Polish Notation. Uses shared pointers to
 *        prevent needless copies of it's potentially large members.
 */
class ParsedInput {
    using OutputQueuePointer = std::shared_ptr<std::queue<std::string>>;
    using VariableMapPointer = std::shared_ptr<std::unordered_map<std::string, Matrix>>;
  public:

    /**
     * @brief Initializes the struct. Both pointers get allocated when this
     *        constructor is called.
     */
    ParsedInput();

    /**
     * @brief A pointer to a queue containing user input parsed to Reverse
     *        Polish Notation.
     */
    OutputQueuePointer output_queue;

    /**
     * @brief A container of parsed temporary values from user input,
     *        as some anonymous values may be used in user input.
     */
    VariableMapPointer loaded_variables;
};
