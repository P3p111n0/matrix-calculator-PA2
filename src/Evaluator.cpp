#include "Evaluator.h"
#include "Matrix.h"
#include "Operator.h"
#include "OperatorLookup.h"
#include <math.h>
#include <stdexcept>

static inline constexpr char RESULT_NAME[] = "__RESULT";

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
        }
    }

    return false;
}

void Evaluator::handle_one_arg(std::stack<std::string> & process_stack,
                               Evaluator::VariableMap & variables,
                               Operator op) const {
    if (process_stack.empty()){
        throw std::runtime_error("Invalid number of arguments.");
    }

    std::string a = process_stack.top();
    process_stack.pop();
    Matrix rhs = variables.at(a);

    std::string result_name(RESULT_NAME);
    result_name += std::to_string(variables.size());

    if (op == Operator::DET){
        auto det = rhs.det();
        if (det.has_value()){
            variables.emplace(result_name, Matrix(det.value(), _factory));
            process_stack.emplace(result_name);
            return;
        }
        throw std::runtime_error("Matrix " + a + "has no determinant");
    }

    if (op == Operator::RANK){
        auto rank = rhs.rank();
        variables.emplace(result_name, Matrix(static_cast<double>(rank), _factory));
        process_stack.push(result_name);
        return;
    }

    switch (op){
    case Operator::TRANSPOSE:
        rhs.transpose();
        process_stack.push(a);
        break;
    case Operator::INV:
        rhs.inverse();
        process_stack.push(a);
        break;
    case Operator::GAUSS:
        rhs.gem();
        process_stack.push(a);
        break;
    case Operator::IMPORT: // todo import
        break;
    case Operator::EXPORT: // todo export
        break;
    case Operator::PRINT:
        std::cout << rhs << std::endl;
        break;
    case Operator::SCAN:
        break;
    }
}

void Evaluator::handle_two_args(std::stack<std::string> &,
                                Evaluator::VariableMap &, Operator) const {}

void Evaluator::handle_five_args(std::stack<std::string> &,
                                 Evaluator::VariableMap &, Operator) const {}
