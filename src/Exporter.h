#pragma once

#include "Matrix.h"
#include "FileHandler.h"
#include <unordered_map>

class Exporter : public FileHandler {
  public:
    Exporter() = default;
    void export_to_file(const std::unordered_map<std::string, Matrix> &,
                        const std::string &);
};
