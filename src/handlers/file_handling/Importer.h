#pragma once

#include "../../matrix_wrapper/Matrix.h"
#include "../../matrix_wrapper/MatrixFactory.h"
#include "FileHandler.h"
#include <string>
#include <unordered_map>

/**
 * @brief Class providing tools to import matrices from JSON files.
 */
class Importer : public FileHandler {
  public:

    /**
     * @brief Initializes the importer and its parent.
     * @param factory Factory used for creating matrices which were read
     *                from files.
     */
    Importer(MatrixFactory factory);

    /**
     * @brief Imports matrices from a JSON file specified by <b>filename</b>.
     * @param[out] out_vars Container, into which the imported matrices
     *                      will be loaded.
     * @param filename Name of the file to import from.
     */
    void import_from_file(std::unordered_map<std::string, Matrix> & out_vars,
                          const std::string & filename);
};
