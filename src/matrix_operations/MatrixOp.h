#pragma once

#include "../matrix_wrapper/Matrix.h"
#include <cstdlib>
#include <string>

/**
 * @brief An abstract base class for matrix operations. Primarily used for
 *        parsing and evaluating user input.
 */
class MatrixOp {
  public:
    /**
     * @brief Initializes the object with the provided attributes.
     * @param arity Arity of the operator.
     * @param priority Priority of the operator. The greater the value, the
     *                 sooner will the operation be evaluated.
     * @param name A unique identifier of the operation.
     */
    MatrixOp(std::size_t arity, std::size_t priority, const std::string & name);
    virtual ~MatrixOp() = default;

    /**
     * @brief A getter for arity.
     * @return Arity of the operator.
     */
    std::size_t arity() const;

    /**
     * @brief A getter for the priority of the operator.
     * @return Precedence of the operator.
     */
    std::size_t priority() const;

    /**
     * @brief A getter for the name of the operator.
     * @return A const reference to the name of the operator.
     */
    const std::string & name() const;

    /**
     * @brief A method evaluating the operation. Might call methods of class
     *        <b>Matrix</b>
     * @param args An array of arguments used for the operation. It's size must
     *             be equal to the <b>arity</b> of the operator. Assumes
     *             arguments in reverse order ie. if the operation takes two
     *             arguments, <b>lhs</b> and <b>rhs</b>, they should be in order
     *             [<b>rhs</b>, <b>lhs</b>].
     * @return Result of the operation.
     */
    virtual Matrix evaluate(const std::vector<Matrix> & args) const = 0;
  protected:
    /**
     * @brief Arity of the operator.
     */
    std::size_t _arity;

    /**
     * @brief Priority of the operator. The greater the value, the sooner will
     *        the operation be evaluated.
     */
    std::size_t _priority;

    /**
     * @brief An identifier of the operation.
     */
    std::string _name;
};
