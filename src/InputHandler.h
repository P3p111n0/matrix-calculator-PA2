#pragma once

#include "MatrixFactory.h"

class InputHandler {
  public:
    explicit InputHandler(MatrixFactory);
  protected:
    MatrixFactory _factory;
    static bool string_has_prefix(const std::string &, const std::string &);
    static inline std::string get_temporary_name(const std::string &, std::size_t);
    static constexpr char RESERVED_NAME_PREFIX[] = "__";
};
