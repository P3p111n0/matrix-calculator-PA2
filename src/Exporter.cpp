#include "Exporter.h"
#include "../libs/json.hpp"

using json = nlohmann::json;

void Exporter::export_to_file(const std::unordered_map<std::string, Matrix> & vars,
                              const std::string & filename) {

}

bool Exporter::good() const { return !_is_failed; }

bool Exporter::bad() const { return _is_failed; }

const std::string & Exporter::status() const { return _status; }
