#pragma once

#include "../../matrix_wrapper/Matrix.h"
#include "../../matrix_wrapper/MatrixFactory.h"
#include "../file_handling/Exporter.h"
#include "../file_handling/Importer.h"
#include "ContainerOperations.h"
#include "InputHandler.h"
#include "ParsedInput.h"
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>

/**
 * @brief A class for evaluating user input parsed in Reverse Polish Notation.
 */
class Evaluator : public InputHandler {
    using VariableMap = std::unordered_map<std::string, Matrix>;
  public:

    /**
     * @brief Initializes the evaluator.
     * @param factory Factory used for creating temporary matrices as a result
     *                of sub-expressions.
     * @param output A stream into which the result will be printed.
     */
    Evaluator(MatrixFactory factory, std::ostream & output);

    /**
     * @brief Evaluates the provided user input.
     * @param input Input parsed to Reverse Polish Notation. See
     *              <b>ParsedInput</b> struct for more information.
     * @throws std::runtime_error if a parsed token cannot be recognized -
     *                            if a token is neither a variable or an
     *                            operator
     * @throws std::runtime_error if <b>MatrixOpPrint</b>, <b>MatrixOpExport</b>,
     *                            <b>MatrixOpImport</b> are used without
     *                            arguments.
     * @throws std::runtime_error if no operator is found in an expression
     *                            containing more than one token.
     * @throws std::runtime_error if more than token is left after evaluation.
     * @throws std::runtime_error if <b>MatrixOp::evaluate(
     *                            const std::vector<Matrix> &)</b> throws.
     * @throws std::invalid_argument if an assignment to a temporary value is
     *                               attempted.
     * @throws std::runtime_error if assignment is called with less than two
     *                            arguments.
     */
    void evaluate_input(const ParsedInput & input);

  private:

    /**
     * @brief Stream into which the results will be printed.
     */
    std::ostream & _stream;

    /**
     * @brief A map of all lasting variables.
     */
    VariableMap _vars;

    /**
     * @brief An exporter used for writing variables to files when an "EXPORT"
     *        token is detected.
     */
    Exporter _exporter;

    /**
     * @brief An importer used for loading variables from files when an "IMPORT"
     *        token is detected.
     */
    Importer _importer;

    /**
     * @brief A helper function used for copying lasting variables encountered
     *        in user input over to <b>_vars</b>.
     */
    void load_nontmp_vars(const VariableMap & temp_vars);

    /**
     * @brief Prints newly available variables after importing from a file.
     */
    void print_available_vars() const;
};
