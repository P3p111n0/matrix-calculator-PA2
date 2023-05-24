#pragma once

#include <functional>
#include <string>
#include "../../matrix_wrapper/Matrix.h"

struct ContainerOperations {
    ContainerOperations(
        const std::function<Matrix &(const std::string &)> & getter,
        const std::function<void(const std::string &, const Matrix &)> &
            replacer)
        : get_var(getter), replace_var(replacer) {}
    std::function<Matrix &(const std::string &)> get_var;
    std::function<void(const std::string &, const Matrix &)> replace_var;
};