#pragma once

#include <cstdlib>
#include <iostream>

/**
 * @brief <b>Configurator</b> provides tools for configuring the calculator.
 *        Presumes configuration files in JSON format.
 */
class Configurator {
  public:

    /**
     * @brief Initializes the configurator.
     * @param logging_stream Stream for logging progress of configuring the
     *                       calculator.
     */
    explicit Configurator(std::ostream & logging_stream);

    /**
     * @brief Loads values from a config file specified by <b>filename</b>.
     *        Loaded values are stored in member variables with the same name
     *        as their JSON tokens.
     * @param filename Name of the configuration JSON file.
     */
    void load_config(const char * filename);

    /**
     * @brief Ratio of elements equal to zero in the matrix to the non-zeroes
     *        for a matrix to be considered sparse. Its value ranges between
     *        [0, 1]. A matrix is considered sparse if it has at least
     *        <b>sparse_ratio</b>*<b>total_number_of_elements</b> elements equal
     *        to zero.
     */
    double sparse_ratio;

    /**
     * @brief Maximum length of every expression in input.
     */
    std::size_t max_input_length;
  private:

    /**
     * @brief Output stream used for logging the configuration process.
     */
    std::ostream & _stream;

    /**
     * @brief Prints default values for every member variable into the provided
     *        stream.
     * @param stream Stream to print the values into.
     */
    void print_defaults(std::ostream & stream) const;

    /**
     * @brief Resets member variables to their default values.
     *        Defaults are:\n
     *        <b>sparse_ratio = 0.5</b>\n
     *        <b>max_input_len = 500</b>
     */
    void set_defaults();
};

