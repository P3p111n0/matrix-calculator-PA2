#pragma once

#include "MatrixFactory.h"

class InputHandler {
  public:
    explicit InputHandler(MatrixFactory);
  protected:
    MatrixFactory _factory;
    static bool string_has_prefix(const std::string &, const std::string &);
};
