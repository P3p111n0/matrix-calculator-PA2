#pragma once

#include "Matrix.h"
#include <unordered_map>
#include <string>

class Exporter {
  public:
    Exporter() = default;
    void export_to_file(const std::unordered_map<std::string, Matrix> &,
                        const std::string &);
    bool good() const;
    bool bad() const;
    const std::string & status() const;
  private:
    std::string _status;
    bool _is_failed;
};
