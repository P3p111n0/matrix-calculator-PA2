#pragma once

#include "../../matrix_wrapper/Matrix.h"
#include "../../matrix_wrapper/MatrixFactory.h"
#include "FileHandler.h"
#include <string>
#include <unordered_map>

class Importer : public FileHandler {
  public:
    Importer(MatrixFactory);
    void import_from_file(std::unordered_map<std::string, Matrix> &, const std::string &);
};
