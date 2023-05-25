#include "Configurator.h"
#include <exception>
#include <filesystem>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include "../../../libs/json.hpp"
#include <limits>

Configurator::Configurator(std::ostream & stream) : _stream(stream) {
    set_defaults();
}

inline const std::vector<std::string> required_attrs {
    "sparse_ratio",
    "max_input_length"
};

using json = nlohmann::json;

static bool check_config(const json & data, const std::vector<std::string> & attrs){
    for (const auto & attr : attrs){
        if (!data.contains(attr) || !data[attr].is_number()){
            return false;
        }
    }
    return true;
}

void Configurator::load_config(const char * file_name) {
    if (!std::filesystem::is_regular_file(file_name)) {
        _stream << "Provided config file is not a regular file, defaulting to: "
                << std::endl;
        set_defaults();
        print_defaults(_stream);
        return;
    }
    std::ifstream file(file_name);
    if (!file.is_open()) {
        _stream << "Couldn't open configuration file, defaulting to: "
                << std::endl;
        print_defaults(_stream);
        return;
    }

    json config_data;
    try {
        config_data = json::parse(file);
    } catch (std::exception & e) {
        _stream << "An error occurred during config parsing. Defaulting to: " << std::endl;
        set_defaults();
        print_defaults(_stream);
        return;
    }
    if (!check_config(config_data, required_attrs)){
        _stream << "One or more attributes are missing in the provided config. Defaulting to: " << std::endl;
        set_defaults();
        print_defaults(_stream);
        return;
    }
    if (config_data.size() > required_attrs.size()){
        _stream << "One or more abundant attributes found in config. Defaulting to: " << std::endl;
        set_defaults();
        print_defaults(_stream);
        return;
    }
    double sparse_r = config_data["sparse_ratio"].get<double>();
    std::size_t max_len = config_data["max_input_length"].get<std::size_t>();

    if (sparse_r < 0 || sparse_r > 1){
        _stream << "Invalid value of sparse_ratio. Defaulting to: " << std::endl;
        set_defaults();
        print_defaults(_stream);
        return;
    }
    if (max_len == 0 || max_len == std::numeric_limits<std::size_t>::max()){
        _stream << "Invalid value of max_input_length. Defaulting to: " << std::endl;
        set_defaults();
        print_defaults(_stream);
        return;
    }

    sparse_ratio = sparse_r;
    max_input_length = max_len;
    _stream << "Config file: OK" << std::endl;
}

void Configurator::print_defaults(std::ostream & os) const {
    os << "\t sparse_ratio = " << sparse_ratio * 100 << "%" << std::endl;
    os << "\t max_input_length = " << max_input_length << std::endl;
}

void Configurator::set_defaults() {
    sparse_ratio = 0.5;
    max_input_length = 500;
}
