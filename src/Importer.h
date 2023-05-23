#pragma once

#include "FileHandler.h"
#include "Matrix.h"
#include "MatrixFactory.h"
#include <unordered_map>
#include <string>


class Importer : public FileHandler {
  public:
    Importer(MatrixFactory);
    void import_from_file(std::unordered_map<std::string, Matrix> &, const std::string &);
};
