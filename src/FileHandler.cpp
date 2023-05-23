#include "FileHandler.h"

bool FileHandler::good() const { return !_is_failed; }

bool FileHandler::bad() const { return _is_failed; }

const std::string & FileHandler::status() const { return _status; }
