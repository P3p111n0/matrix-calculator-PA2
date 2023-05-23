#include "FileHandler.h"

FileHandler::FileHandler(MatrixFactory factory) : BaseHandler(factory) {}

bool FileHandler::good() const { return !_is_failed; }

bool FileHandler::bad() const { return _is_failed; }

const std::string & FileHandler::status() const { return _status; }
