#pragma once

#include "../../matrix_wrapper/Matrix.h"
#include "../../matrix_wrapper/MatrixFactory.h"
#include "InputHandler.h"
#include "ParsedInput.h"
#include <cstdlib>
#include <iostream>

/**
 * @brief A parsing unit used for parsing user input to Reverse Polish Notation.
 */
class Parser : public InputHandler {
  public:

    /**
     * @brief Initializes the parser.
     * @param factory A Factory used for creating variables received in
     *                user input.
     * @param input_stream A stream to read user input from.
     * @param max_input_len Determines the maximum length for a single
     *                      expression in user input.
     */
    Parser(MatrixFactory factory, std::istream & input_stream,
           std::size_t max_input_len);

    /**
     * @brief Parses user input from the member input stream to Reverse
     *        Polish Notation.
     * @return Parsed user input in RPN. See <b>ParsedInput</b> struct for more
     *         information.
     * @throws std::runtime_error if a token containing a reserved name is
     *                            encountered.
     * @throws std::runtime_error if all parenthesis on input aren't matching.
     * @throws std::runtime_error if an operation name is used in call of
     *                            "SCAN"
     * @throws std::runtime_error if a matrix cannot be parsed in user input.
     * @throws std::invalid_argument if a token starts with a number
     * @throws std::ios_base::failure if an expression exceeds the maximum allowed
     *                                limit.
     * @throws std::ios_base::failure if the input stream fails while reading.
     */
    ParsedInput parse_input() const;

  private:

    /**
     * @brief Parsers an inline matrix.
     * @param stream Stream to load the matrix from.
     * @return Parsed inline matrix.
     * @throws std::runtime_error when a mismatch of braces is detected.
     * @throws std::runtime_error if all rows don't have the same number of
     *                            columns.
     * @throws std::runtime_error if the matrix contains other characters apart
     *                            from commas and braces.
     * @throws std::runtime_error if <b>stream</b> fails.
     */
    Matrix load_matrix(std::istream & stream) const;

    /**
     * @brief Parsers a matrix in brace format.
     * @param stream Stream to load the matrix from.
     * @return Parsed matrix.
     * @throws std::runtime_error when a mismatch of braces is detected.
     * @throws std::runtime_error if all rows don't have the same number of
     *                            columns.
     * @throws std::runtime_error if the matrix contains other characters apart
     *                            from commas and braces.
     * @throws std::runtime_error if <b>stream</b> fails.
     */
    Matrix load_matrix_scan(std::istream & stream) const;

    /**
     * @brief Stream from which user input is read.
     */
    std::istream & _stream;

    /**
     * @brief Maximum length of each expression.
     */
    std::size_t _max_len;
};
