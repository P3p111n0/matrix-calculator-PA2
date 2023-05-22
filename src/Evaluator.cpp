#include "Evaluator.h"
#include "Matrix.h"
#include "Operator.h"
#include "OperatorLookup.h"
#include "ParsedInput.h"
#include <functional>
#include <stdexcept>
#include <string>

static inline constexpr char RESULT_NAME[] = "RESULT";

template <typename T> inline T top_and_pop(std::stack<T> & x) {
    T top = x.top();
    x.pop();
    return top;
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

Evaluator::Evaluator(MatrixFactory factory, std::ostream & os)
    : InputHandler(factory), _stream(os) {}

void Evaluator::evaluate_input(const ParsedInput & input) {
    auto & output_queue = *input.output_queue;
    auto loaded_vars = *input.loaded_variables;
    load_nontmp_vars(loaded_vars);
    std::stack<std::string> process_stack;

    auto getter = [&](const std::string & key) {
        if (_vars.count(key)) {
            return _vars.at(key);
        } else if (loaded_vars.count(key)) {
            return loaded_vars.at(key);
        }
        throw std::runtime_error("Unknown token: " + key);
    };

    auto inserter = [&](const std::string & key, const Matrix & value) {
        if (string_has_prefix(key, RESERVED_NAME_PREFIX)) {
            loaded_vars.erase(key);
            loaded_vars.emplace(key, value);
            return;
        }
        _vars.erase(key);
        _vars.emplace(key, value);
    };

    ContainerOperations actions(getter, inserter);

    while (!output_queue.empty()) {
        std::string token = output_queue.front();
        output_queue.pop();

        if (!OPERATOR_LOOKUP.count(token)) {
            process_stack.push(token);
            continue;
        }

        Operator op = OPERATOR_LOOKUP.at(token);
        std::size_t n_of_args = N_OF_ARGS_LOOKUP.at(op);

        switch (n_of_args) {
        case 1:
            handle_one_arg(process_stack, op, actions);
            break;
        case 2:
            handle_two_args(process_stack, op, actions);
            break;
        case 5:
            handle_five_args(process_stack, op, actions);
            break;
        default:
            // this shouldn't happen
            throw std::runtime_error("Unknown number of arguments.");
        }

        if (!process_stack.empty()) {
            std::string leftover = process_stack.top();
            Matrix res = getter(leftover);
            _stream << res << std::endl;
        }
    }
}
void Evaluator::handle_one_arg(std::stack<std::string> & process_stack,
                               Operator op, const ContainerOperations & actions) {
    if (process_stack.empty()) {
        throw std::runtime_error(
            "Invalid number of arguments, 1 expected, 0 provided.");
    }

    std::string a = top_and_pop(process_stack);
    Matrix rhs = actions.get_var(a);
    std::string result_name = get_temporary_name(RESULT_NAME);

    if (!process_stack.empty()) {
        result_name = a;
    }

    switch (op) {
    case Operator::DET: {
        auto det = rhs.det();
        if (!det.has_value()) {
            throw std::runtime_error("Determinant of " + a + "is undefined.");
        }
        rhs = Matrix(det.value(), _factory);
        break;
    }
    case Operator::RANK: {
        auto rank = rhs.rank();
        rhs = Matrix(static_cast<double>(rank), _factory);
        break;
    }
    case Operator::TRANSPOSE:
        rhs.transpose();
        break;
    case Operator::INV:
        rhs.inverse();
        break;
    case Operator::GAUSS:
        rhs.gem();
        break;
    case Operator::IMPORT: // todo import
        break;
    case Operator::EXPORT: // todo export
        break;
    case Operator::PRINT:
        _stream << rhs << std::endl;
        break;
    }

    if (op != Operator::PRINT) {
        actions.replace_var(result_name, rhs);
        process_stack.push(result_name);
    }
}

void Evaluator::handle_two_args(std::stack<std::string> & process_stack,
                                Operator op, const ContainerOperations & actions) {
    if (process_stack.size() < 2) {
        throw std::runtime_error("Invalid number of arguments, 2 expected " +
                                 std::to_string(process_stack.size()) +
                                 " provided");
    }

    std::string b = top_and_pop(process_stack);
    std::string a = top_and_pop(process_stack);

    if (op == Operator::ASSIGN && !process_stack.empty()) {
        throw std::runtime_error("Invalid use of '=' inside an expression.");
    }

    std::string result_name = get_temporary_name(RESULT_NAME);
    Matrix rhs = actions.get_var(b);

    if (op == Operator::ASSIGN){
        actions.replace_var(a, rhs);
        return;
    }

    Matrix lhs = actions.get_var(a);

    switch (op) {
    case Operator::PLUS:
        actions.replace_var(result_name, lhs + rhs);
        break;
    case Operator::MINUS:
        actions.replace_var(result_name, lhs - rhs);
        break;
    case Operator::MUL:
        actions.replace_var(result_name, lhs * rhs);
        break;
    case Operator::UNITE:
        actions.replace_var(result_name, Matrix::unite(lhs, rhs));
        break;
    }
    process_stack.push(result_name);
}

void Evaluator::handle_five_args(std::stack<std::string> & process_stack,
                                 Operator op, const ContainerOperations & actions) {
    if (process_stack.size() < 5) {
        throw std::runtime_error("Invalid number of arguments, 5 expected " +
                                 std::to_string(process_stack.size()) +
                                 " provided");
    }

    std::string offset_column_token = top_and_pop(process_stack);
    std::string offset_row_token = top_and_pop(process_stack);
    std::string new_columns_token = top_and_pop(process_stack);
    std::string new_rows_token = top_and_pop(process_stack);
    std::string rhs_token = top_and_pop(process_stack);

    // might have to check for existing tokens
    Matrix rhs = actions.get_var(rhs_token);
    Matrix offset_row_matrix = actions.get_var(offset_row_token);
    Matrix offset_column_matrix = actions.get_var(offset_column_token);
    Matrix new_rows_matrix = actions.get_var(new_rows_token);
    Matrix new_columns_matrix = actions.get_var(new_columns_token);

    std::string result_name = get_temporary_name(RESULT_NAME);
    if (process_stack.empty()) {
        result_name = rhs_token;
    }

    switch (op) {
    case Operator::CUT:
        rhs.cut(new_rows_matrix, new_columns_matrix, offset_row_matrix,
                offset_column_matrix);
        break;
    }

    actions.replace_var(result_name, rhs);
    process_stack.emplace(result_name);
}