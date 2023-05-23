#pragma once

#include <string>

class FileHandler {
  public:
    FileHandler() = default;
    bool good() const;
    bool bad() const;
    const std::string & status() const;
  protected:
    std::string _status;
    bool _is_failed;
};
