#pragma once

#include "../handlers/config_handling/Configurator.h"
#include "../handlers/input_handling/Evaluator.h"
#include "../handlers/input_handling/Parser.h"
#include <iostream>

/**
 * @brief Main class of the project. Implements a matrix calculator.
 */
class MatrixCalculator {
  public:

    /**
     * @brief Initializes the calculator.
     * @param input Stream containing user input.
     * @param result_stream Stream used for printing results of the
     *                      calculations.
     * @param config An optional parameter for specifying a configuration file.
     */
    MatrixCalculator(std::istream & input, std::ostream & result_stream,
                     const std::string & config = "");

    /**
     * @brief Starts the main loop. Reads, parses and evaluates user input
     *        until EOF is reached.
     */
    void start();

  private:

    /**
     * @brief A configuration unit used to tweak the matrix creation process
     *        and maximum allowed length of user input.
     */
    Configurator _config;

    /**
     * @brief Stream for reading user input.
     */
    std::istream & _in;

    /**
     * @brief Stream into which results of the calculations are printed.
     */
    std::ostream & _out;
};
