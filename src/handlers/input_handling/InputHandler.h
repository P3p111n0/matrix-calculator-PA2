#pragma once

#include "../../matrix_wrapper/Matrix.h"
#include "../../matrix_wrapper/MatrixFactory.h"
#include "../BaseHandler.h"
#include <unordered_map>

class InputHandler : public BaseHandler {
  public:
    explicit InputHandler(MatrixFactory);
  protected:
    static bool string_has_prefix(const std::string &, const std::string &);
    std::string get_temporary_name(const std::string &) const;
    static constexpr char RESERVED_NAME_PREFIX[] = "__";
};
