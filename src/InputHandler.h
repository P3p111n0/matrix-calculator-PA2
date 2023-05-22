#pragma once

#include <unordered_map>
#include "MatrixFactory.h"
#include "Matrix.h"

class InputHandler {
  public:
    explicit InputHandler(MatrixFactory);
  protected:
    MatrixFactory _factory;

    static bool string_has_prefix(const std::string &, const std::string &);
    static std::string get_temporary_name(const std::string &, std::size_t);
    static constexpr char RESERVED_NAME_PREFIX[] = "__";
};
