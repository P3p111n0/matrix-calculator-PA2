#include "Evaluator.h"
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

        if (special_case_table.count(token)) {
            switch (special_case_table.at(token)) {
            case SpecialCases::PRINT: {
                auto arg = get_args(process_stack, actions, 1);
                _stream << arg[0] << std::endl;
                return;
            }
            case SpecialCases::EXPORT:{
                const std::string & filename = process_stack.top();
                _exporter.export_to_file(_vars, filename);
                _stream << _exporter.status() << std::endl;
                return;
            }
            case SpecialCases::IMPORT:{
                const std::string & filename = process_stack.top();
                _importer.import_from_file(_vars, filename);
                _stream << _importer.status() << std::endl;
                return;
            }
            case SpecialCases::ASSIGN:{
                auto arg = get_args(process_stack, actions, 1);
                const std::string & dest = process_stack.top();
                _vars.erase(dest);
                _vars.emplace(dest, std::move(arg[0]));
                return;
            }
            }
        }

        if (!op_factory.is_operation(token)) {
            process_stack.push(token);
            continue;
        }

        std::unique_ptr<MatrixOp> operation(op_factory.get_operation(token));
        if (process_stack.size() < operation->arity()){
            _stream << "Not enough arguments for operation: " << token << std::endl;
            return;
        }
        Matrix result = operation->evaluate(
            get_args(process_stack, actions, operation->arity()));
        std::string temporary_name = get_temporary_name(RESULT_NAME);
        temp_vars.emplace(temporary_name, std::move(result));
        process_stack.push(temporary_name);
    }
    if (!process_stack.empty()) {
        std::string leftover = process_stack.top();
        Matrix res = actions.get_var(leftover);
        _stream << res << std::endl;
    }
}