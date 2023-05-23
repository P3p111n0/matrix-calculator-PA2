#pragma once

#include "BaseHandler.h"
#include <string>

class FileHandler : public BaseHandler {
  public:
    FileHandler(MatrixFactory);
    bool good() const;
    bool bad() const;
    const std::string & status() const;
  protected:
    std::string _status;
    bool _is_failed;

    void reset();
};
