#pragma once

#include <functional>
#include <string>
#include "../../matrix_wrapper/Matrix.h"

struct ContainerOperations {
    explicit ContainerOperations(
        const std::function<Matrix &(const std::string &)> & getter)
        : get_var(getter) {}
    std::function<Matrix &(const std::string &)> get_var;
};