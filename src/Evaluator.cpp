#include "Evaluator.h"
#include "Matrix.h"
#include "OperatorLookup.h"
#include <string>

static inline constexpr char RESULT_NAME[] = "__RESULT";

static inline std::string get_result_name(std::size_t map_size) {
    return std::string(RESULT_NAME) += std::to_string(map_size);
}

template <typename T> inline T top_and_pop(std::stack<T> & x) {
    T top = x.top();
    x.pop();
    return top;
}

Evaluator::Evaluator(MatrixFactory factory) : _factory(factory) {}

bool Evaluator::evaluate_input(
    Evaluator::VariableMap & variables,
    Evaluator::OutputQueue & output_queue_pointer) const {
    auto & output_queue = *output_queue_pointer;
    std::stack<std::string> process_stack;

    while (!output_queue.empty()) {
        std::string token = output_queue.front();
        output_queue.pop();

        if (!(OPERATOR_LOOKUP.count(token) || FUNCTION_LOOKUP.count(token))) {
            process_stack.push(token);
            continue;
        }

        Operator op = FULL_OPERATOR_LOOKUP.at(token);
        std::size_t n_of_args = N_OF_ARGS_LOOKUP.at(op);

        switch (n_of_args) {
        case 1:
            handle_one_arg(process_stack, variables, op);
            break;
        case 2:
            handle_two_args(process_stack, variables, op);
            break;
        case 5:
            handle_five_args(process_stack, variables, op);
            break;
        default:
            // this shouldn't happen
            throw std::runtime_error("Unknown number of arguments.");
        }
    }

    if (!process_stack.empty()) {
        std::string leftover = process_stack.top();
        Matrix res = variables.at(leftover);
        std::cout << res << std::endl;
    }

    return true;
}

void Evaluator::handle_one_arg(std::stack<std::string> & process_stack,
                               Evaluator::VariableMap & variables,
                               Operator op) const {
    if (process_stack.empty()) {
        throw std::runtime_error(
            "Invalid number of arguments, 1 expected, 0 provided.");
    }

    std::string a = top_and_pop(process_stack);
    if (!variables.count(a)) {
        throw std::invalid_argument("Invalid token: " + a);
    }

    Matrix rhs = variables.at(a);
    std::string result_name = get_result_name(variables.size());

    if (!process_stack.empty()) {
        result_name = a;
    }

    if (op == Operator::DET) {
        auto det = rhs.det();
        if (det.has_value()) {
            variables.emplace(result_name, Matrix(det.value(), _factory));
            process_stack.emplace(result_name);
            return;
        }
        throw std::runtime_error("Matrix " + a + "has no determinant");
    }

    if (op == Operator::RANK) {
        auto rank = rhs.rank();
        variables.emplace(result_name,
                          Matrix(static_cast<double>(rank), _factory));
        process_stack.push(result_name);
        return;
    }

    switch (op) {
    case Operator::TRANSPOSE:
        rhs.transpose();
        process_stack.push(result_name);
        break;
    case Operator::INV:
        rhs.inverse();
        process_stack.push(result_name);
        break;
    case Operator::GAUSS:
        rhs.gem();
        process_stack.push(result_name);
        break;
    case Operator::IMPORT: // todo import
        break;
    case Operator::EXPORT: // todo export
        break;
    case Operator::PRINT:
        std::cout << rhs << std::endl;
        break;
    }

    if (op != Operator::PRINT) {
        variables.erase(result_name);
        variables.emplace(result_name, rhs);
    }
}
void Evaluator::handle_two_args(std::stack<std::string> & process_stack,
                                Evaluator::VariableMap & variables,
                                Operator op) const {
    if (process_stack.size() < 2) {
        throw std::runtime_error("Invalid number of arguments, 2 expected " +
                                 std::to_string(process_stack.size()) +
                                 " provided");
    }

    std::string b = top_and_pop(process_stack);
    std::string a = top_and_pop(process_stack);

    if (!variables.count(b)) {
        throw std::invalid_argument("Unknown token: " + b);
    }

    std::string result_name = get_result_name(variables.size());

    Matrix rhs = variables.at(b);
    if (auto a_count = variables.count(a);
        !a_count &&
        op == Operator::ASSIGN) { // temporary for the ASSIGN operator
        variables.emplace(a, Matrix(0, _factory));
        result_name = a;
    } else if (!a_count) {
        throw std::invalid_argument("Unknown token: " + a);
    }

    Matrix lhs = variables.at(a);

    switch (op) {
    case Operator::PLUS:
        variables.emplace(result_name, lhs + rhs);
        process_stack.push(result_name);
        break;
    case Operator::MINUS:
        variables.emplace(result_name, lhs - rhs);
        process_stack.push(result_name);
        break;
    case Operator::ASSIGN:
        variables.erase(result_name);
        variables.emplace(result_name, rhs);
        break;
    case Operator::MUL:
        variables.emplace(result_name, lhs * rhs);
        process_stack.push(result_name);
        break;
    case Operator::UNITE:
        variables.emplace(result_name, Matrix::unite(lhs, rhs));
        process_stack.push(a);
        break;
    }
}

void Evaluator::handle_five_args(std::stack<std::string> & process_stack,
                                 Evaluator::VariableMap & variables, Operator op) const {
    if (process_stack.size() < 5){
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
    Matrix rhs = variables.at(rhs_token);
    Matrix offset_row_matrix = variables.at(offset_row_token);
    Matrix offset_column_matrix = variables.at(offset_column_token);
    Matrix new_rows_matrix = variables.at(new_rows_token);
    Matrix new_columns_matrix = variables.at(new_columns_token);

    std::string result_name = get_result_name(variables.size());
    if (process_stack.empty()){
        result_name = rhs_token;
    }

    switch(op){
    case Operator::CUT:
        rhs.cut(new_rows_matrix, new_columns_matrix, offset_row_matrix, offset_column_matrix);
        break;
    }

    variables.erase(result_name);
    variables.emplace(result_name, rhs);
    process_stack.emplace(result_name);
}
