#pragma once

#include "../../matrix_wrapper/Matrix.h"
#include "FileHandler.h"
#include <unordered_map>

/**
 * @brief Class serving as a way to export matrices to files. Exports matrices
 *        to JSON files.
 */
class Exporter : public FileHandler {
  public:

    /**
     * @brief Initializes the parent Exporter and it's parent.
     * @param factory Factory used for determining the format for exporting a
     *                certain matrix.
     */
    Exporter(MatrixFactory factory);

    /**
     * @brief Exports matrices to a JSON file.
     * @param vars A container of variables to export.
     * @param filename Name of the resulting file.
     */
    void export_to_file(const std::unordered_map<std::string, Matrix> & vars,
                        const std::string & filename);
};
