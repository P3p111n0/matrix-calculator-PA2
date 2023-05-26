#pragma once

#include <functional>
#include <string>
#include "../../matrix_wrapper/Matrix.h"

/**
 * @brief A container for operations with multiple matrix containers. Allows
 *        for more memory efficient operations in user input evaluation.
 */
struct ContainerOperations {

    /**
     * @brief Initializes the container.
     * @param getter A function for retrieving matrices from various containers.
     */
    explicit ContainerOperations(
        const std::function<Matrix &(const std::string &)> & getter)
        : get_var(getter) {}

    /**
     * @brief A function for getting matrices from multiple containers.
     *        May throw.
     */
    std::function<Matrix &(const std::string &)> get_var;
};