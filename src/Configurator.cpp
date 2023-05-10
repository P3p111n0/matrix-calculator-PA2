#include "Configurator.h"
#include <fstream>
#include <ostream>
#include <set>
#include <sstream>
#include <string>

Configurator::Configurator(std::ostream * stream) : _stream(stream) {
    set_defaults();
}

void Configurator::load_config(const char * file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        *_stream << "Couldn't open configuration file, defaulting to: "
                 << std::endl;
        print_defaults(*_stream);
        return;
    }

    std::set<std::string> set_values;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()){
            continue;
        }

        // 2 is the minimal line length
        if (line.length() < 2) {
            syntax_error(*_stream);
            return;
        }

        // skip comments
        if (line[0] == '/' && line[1] == '/') {
            continue;
        }

        std::stringstream line_stream(line);
        std::string config_val;
        line_stream >> config_val;

        if (set_values.count(config_val)){
            *_stream << "Multiple definitions of: " << config_val << std::endl;
            *_stream << "Defaulting to: " << std::endl;
            set_defaults();
            print_defaults(*_stream);
            return;
        }

        set_values.emplace(config_val);

        if (config_val == "SPARSE_RATIO") {
            char symbol;
            double numerator, denominator;

            line_stream >> symbol;
            if (symbol != '=' || line_stream.bad()) {
                error_and_reset(*_stream);
                return;
            }

            line_stream >> numerator;

            line_stream >> symbol;
            if (symbol != '/' || line_stream.bad()) {
                error_and_reset(*_stream);
                return;
            }
            line_stream >> denominator;
            if (line_stream.bad()) {
                error_and_reset(*_stream);
                return;
            }

            _ratio = numerator / denominator;
            continue;
        }

        if (config_val == "MAX_INPUT_LEN"){
            char symbol;
            std::size_t new_len;

            line_stream >> symbol;
            if (symbol != '=' || line_stream.bad()){
                error_and_reset(*_stream);
                return;
            }

            line_stream >> new_len;
            if (line_stream.bad()){
                error_and_reset(*_stream);
                return;
            }

            _max_len = new_len;
            continue;
        }

        *_stream << "Unknown parameter: " << config_val << std::endl;
        *_stream << "Defaulting to: " << std::endl;
        set_defaults();
        print_defaults(*_stream);
        return;
    }

    *_stream << "Config file: OK" << std::endl;
}

void Configurator::error_and_reset(std::ostream & os) {
    syntax_error(os);
    set_defaults();
}

void Configurator::syntax_error(std::ostream & os) const {
    os << "Syntax error in configuration file, defaulting to:"
             << std::endl;
    print_defaults(os);
}

void Configurator::print_defaults(std::ostream & os) const {
    os << "\t SPARSE_RATIO = " << _ratio * 100 << "%" << std::endl;
    os << "\t MAX_INPUT_LEN = " << _max_len << std::endl;
}

void Configurator::set_defaults() {
    _ratio = 2/3.0;
    _max_len = 200;
}
