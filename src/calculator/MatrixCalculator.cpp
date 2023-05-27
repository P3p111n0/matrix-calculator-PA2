#include "MatrixCalculator.h"
#include <exception>

MatrixCalculator::MatrixCalculator(std::istream & input, std::ostream & output,
                                   const std::string & config_file)
    : _config(output), _in(input), _out(output) {
    if (!config_file.empty()) {
        _config.load_config(config_file.c_str());
    }
}

void MatrixCalculator::start() {
    Parser parser(MatrixFactory(_config.sparse_ratio), _in,
                  _config.max_input_length);
    Evaluator evaluator(MatrixFactory(_config.sparse_ratio), _out);
    std::string prefix;
    while (!_in.eof()) {
        if (!_in.good()){
            _out << "Corrupted data detected on input, quitting." << std::endl;
            return;
        }
        try {
            _out << prefix << ">>> ";
            auto parsed_input = parser.parse_input();
            evaluator.evaluate_input(parsed_input);
            prefix.clear();
        } catch (std::exception & e){
            _out << e.what() << std::endl;
            prefix = "!**";
            continue;
        }
    }
}
