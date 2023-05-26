#pragma once

#include "MatrixOp.h"
#include <memory>
#include <set>
#include <unordered_map>

/**
 * @brief A factory class for creating operations. Mainly used for parsing
 *        and evaluating user input.
 */
class OperationFactory {
  public:
    /**
     * @brief Initializes the factory and it's built-in operations.
     */
    OperationFactory();

    /**
     * @brief Getter for operations identified by their name.
     * @param name Name of the operation.
     * @return A shared pointer to the operation.
     * @throws std::invalid_argument if no operation with such name can be
     *                               found.
     */
    std::shared_ptr<MatrixOp> get_operation(const std::string & name) const;

    /**
     * @brief A method used to find out, whether <b>name</b> is an identifier
     *        of an operation or not.
     * @param name Name of the token to be examined.
     * @return True if <b>name</b> identifies a supported operation, false
     *         otherwise.
     */
    bool is_operation(const std::string & name) const;

    /**
     * @brief Getter for the priority of an operation identified by it's name.
     *        The greater the returned value is, the greater is the priority
     *        of the operation.
     * @param name Name of the operation.
     * @return The precedence of the operation in question.
     * @throws std::invalid_argument if <b>name</b> is not a supported operation.
     */
    std::size_t priority_of(const std::string & name) const;
  private:
    /**
     * @brief A lookup table for operations.\n
     *        <b>Key</b>: - a unique identifier of the operation.\n
     *        <b>Value</b>: - A shared pointer to the operation. A shared
     *                        pointer is used to prevent needless allocations.
     */
    std::unordered_map<std::string, std::shared_ptr<MatrixOp>> _operations;
};
