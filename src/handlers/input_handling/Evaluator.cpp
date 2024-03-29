#include "Evaluator.h"
#include "../../exceptions/QuitSignal.h"
#include "../../matrix_operations/OperationFactory.h"
#include "ContainerOperations.h"
#include "ParsedInput.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

static inline constexpr char RESULT_NAME[] = "RESULT";

static std::vector<Matrix> get_args(std::stack<std::string> & token_stack,
                                    const ContainerOperations & actions,
                                    std::size_t n) {
    std::vector<Matrix> res;
    for (std::size_t i = 0; i < n; i++) {
        std::string token = token_stack.top();
        token_stack.pop();
        res.emplace_back(actions.get_var(token));
    }
    return res;
}

void Evaluator::load_nontmp_vars(const Evaluator::VariableMap & tmps) {
    for (const auto & [key, val] : tmps) {
        if (!string_has_prefix(key, RESERVED_NAME_PREFIX)) {
            if (_vars.count(key)) {
                _stream << "Warning: Redefinition of variable: " << key
                        << std::endl;
                _vars.erase(key);
            }
            _vars.emplace(key, val);
        }
    }
}

void Evaluator::print_available_vars() const {
    _stream << "Available variables: " << std::endl;
    std::size_t cnt = 0;
    for (const auto & [key, val] : _vars){
        _stream << key;
        ++cnt;
        if (cnt != _vars.size()){
            _stream << ",";
        }
    }
    _stream << std::endl;
}

enum class SpecialCases { PRINT, EXPORT, IMPORT, ASSIGN };

inline const std::unordered_map<std::string, SpecialCases> special_case_table =
    {{"PRINT", SpecialCases::PRINT},
     {"EXPORT", SpecialCases::EXPORT},
     {"IMPORT", SpecialCases::IMPORT},
     {"=", SpecialCases::ASSIGN}};

Evaluator::Evaluator(MatrixFactory factory, std::ostream & os)
    : InputHandler(factory), _stream(os), _exporter(factory),
      _importer(factory) {}

void Evaluator::evaluate_input(const ParsedInput & input) {
    auto & output_queue = *input.output_queue;
    auto & temp_vars = *input.loaded_variables;
    load_nontmp_vars(temp_vars);
    std::stack<std::string> process_stack;
    OperationFactory op_factory;
    bool an_operator_occurred = false;

    auto getter = [&](const std::string & key) -> Matrix & {
        if (_vars.count(key)) {
            return _vars.at(key);
        } else if (temp_vars.count(key)) {
            return temp_vars.at(key);
        }
        throw std::runtime_error("Unknown token: " + key);
    };

    ContainerOperations actions(std::move(getter));

    while (!output_queue.empty()) {
        std::string token = output_queue.front();
        output_queue.pop();

        if (token == "QUIT") {
            if (!output_queue.empty() || !process_stack.empty()){
                throw std::runtime_error("Invalid use of QUIT.");
            }
            throw QuitSignal();
        }

        if (special_case_table.count(token)) {
            an_operator_occurred = true;
            switch (special_case_table.at(token)) {
            case SpecialCases::PRINT: {
                if (process_stack.size() != 1) {
                    throw std::runtime_error("Invalid use of PRINT.");
                }
                auto arg = get_args(process_stack, actions, 1);
                _stream << arg[0] << std::endl;
                continue;
            }
            case SpecialCases::EXPORT: {
                if (process_stack.size() != 1) {
                    throw std::runtime_error("Invalid use of EXPORT.");
                }
                const std::string & filename = process_stack.top();
                _exporter.export_to_file(_vars, filename);
                _stream << _exporter.status() << std::endl;
                process_stack.pop();
                return;
            }
            case SpecialCases::IMPORT: {
                if (process_stack.size() != 1) {
                    throw std::runtime_error("Invalid use of IMPORT.");
                }
                const std::string & filename = process_stack.top();
                _importer.import_from_file(_vars, filename);
                _stream << _importer.status() << std::endl;
                process_stack.pop();
                if (_importer.good()) {
                    print_available_vars();
                }
                return;
            }
            case SpecialCases::ASSIGN: {
                if (process_stack.size() < 2) {
                    throw std::runtime_error(
                        "Invalid number of arguments for operation '='.");
                }
                auto arg = get_args(process_stack, actions, 1);
                const std::string & dest = process_stack.top();
                if (string_has_prefix(dest, RESERVED_NAME_PREFIX)) {
                    throw std::invalid_argument(
                        "Invalid identifier used in assignment.");
                }
                if (_vars.count(dest)) {
                    _stream << "Warning: Redefinition of variable: " << dest
                            << std::endl;
                }
                _vars.erase(dest);
                _vars.emplace(dest, std::move(arg[0]));
                if (process_stack.size() == 1) {
                    process_stack.pop();
                }
                continue;
            }
            }
        }

        if (!op_factory.is_operation(token)) {
            process_stack.push(token);
            continue;
        }

        an_operator_occurred = true;
        std::shared_ptr<MatrixOp> operation(op_factory.get_operation(token));
        if (process_stack.size() < operation->arity()) {
            _stream << "Not enough arguments for operation: " << token
                    << std::endl;
            return;
        }
        Matrix result = operation->evaluate(
            get_args(process_stack, actions, operation->arity()));
        std::string temporary_name = get_temporary_name(RESULT_NAME);
        temp_vars.emplace(temporary_name, std::move(result));
        process_stack.push(temporary_name);
    }
    if (!process_stack.empty()) {
        if (process_stack.size() == 1) {
            std::string leftover = process_stack.top();
            Matrix res = actions.get_var(leftover);
            _stream << res << std::endl;
        } else if (!an_operator_occurred) {
            throw std::runtime_error("No operator has been found.");
        } else {
            throw std::runtime_error("An unknown evaluation error occurred.");
        }
    }
}
