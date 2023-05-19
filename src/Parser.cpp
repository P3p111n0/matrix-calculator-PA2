#include "Parser.h"
#include "DenseMatrixIterator.h"
#include "Matrix.h"
#include "MatrixDimensions.h"
#include "MatrixFactory.h"
#include "Operator.h"
#include <queue>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

inline constexpr char TMP_NAME[] = "__TMP";
static const std::unordered_map<std::string, Operator> OPERATOR_LOOKUP{
    {"+", Operator::PLUS},
    {"-", Operator::MINUS},
    {"=", Operator::ASSIGN},
    {"*", Operator::MUL},
    {"UNITE", Operator::UNITE},
    {"CUT", Operator::CUT},
    {"(", Operator::LEFT_PARENTHESIS},
    {")", Operator::RIGHT_PARENTHESIS},
    {"[", Operator::MATRIX_BRACE_LEFT},
    {"]", Operator::MATRIX_BRACE_RIGHT}};

static const std::unordered_map<std::string, Operator> FUNCTION_LOOKUP{
    {"TRANSPOSE", Operator::TRANSPOSE},
    {"INV", Operator::INV},
    {"DET", Operator::DET},
    {"RANK", Operator::RANK},
    {"GAUSS", Operator::GAUSS},
    {"IMPORT", Operator::IMPORT},
    {"EXPORT", Operator::EXPORT},
    {"PRINT", Operator::PRINT},
    {"SCAN", Operator::SCAN}};

static const std::unordered_map<std::string, OperatorPriority> PRIORITY_LOOKUP{
    {"+", OperatorPriority::PLUS},
    {"-", OperatorPriority::MINUS},
    {"=", OperatorPriority::ASSIGN},
    {"*", OperatorPriority::MUL},
    {"UNITE", OperatorPriority::UNITE},
    {"CUT", OperatorPriority::CUT},
    {"(", OperatorPriority::LEFT_PARENTHESIS},
    {")", OperatorPriority::RIGHT_PARENTHESIS},
    {"[", OperatorPriority::MATRIX_BRACE_LEFT},
    {"]", OperatorPriority::MATRIX_BRACE_RIGHT},
    {"TRANSPOSE", OperatorPriority::TRANSPOSE},
    {"INV", OperatorPriority::INV},
    {"DET", OperatorPriority::DET},
    {"RANK", OperatorPriority::RANK},
    {"GAUSS", OperatorPriority::GAUSS},
    {"IMPORT", OperatorPriority::IMPORT},
    {"EXPORT", OperatorPriority::EXPORT},
    {"PRINT", OperatorPriority::PRINT},
    {"SCAN", OperatorPriority::SCAN}};

Parser::Parser(std::istream & stream, std::size_t max_input_len, const MatrixFactory & factory)
    : _stream(stream), _max_len(max_input_len), _factory(factory) {}

std::shared_ptr<std::queue<std::string>> Parser::parse_input(
    std::unordered_map<std::string, Matrix> & variables) const {
    std::shared_ptr<std::queue<std::string>> output_queue = std::make_shared<std::queue<std::string>>();
    std::stack<std::string> operator_stack;
    std::string line;

    std::getline(_stream, line);
    if (line.length() > _max_len) {
        throw std::runtime_error("Max input length exceeded");
    }

    std::stringstream line_stream(line);
    while (!line_stream.eof()) {
        line_stream >> std::ws;
        std::string token;

        if (line_stream.peek() == '['){
            char c;
            line_stream >> c;
            Matrix parsed_matrix = load_matrix(line_stream);
            std::string new_name = TMP_NAME;
            new_name += std::to_string(variables.size());
            variables.emplace(new_name, parsed_matrix);
            output_queue->push(new_name);
            continue;
        }

        line_stream >> token;

        // token is a variable
        if (variables.count(token)) {
            output_queue->push(token);
            continue;
        }

        // token is a single argument function
        if (FUNCTION_LOOKUP.count(token)) {
            if (operator_stack.size() > 1) {
                throw std::runtime_error(
                    "Functions cannot be used in compound expressions.");
            }
            operator_stack.push(token);
            continue;
        }

        if (token == "(") {
            operator_stack.push(token);
            continue;
        }

        if (token == ")") {
            if (operator_stack.empty()) {
                throw std::runtime_error("Parenthesis mismatch on input");
            }
            while (operator_stack.top() != "(") {
                output_queue->push(operator_stack.top());
                operator_stack.pop();
                if (operator_stack.empty()) {
                    throw std::runtime_error("Parenthesis mismatch on input");
                }
            }

            if (!operator_stack.empty() && operator_stack.top() != "(") {
                throw std::runtime_error("Parenthesis mismatch on input");
            }
            operator_stack.pop();

            if (FUNCTION_LOOKUP.count(operator_stack.top())) {
                output_queue->push(operator_stack.top());
                operator_stack.pop();
            }

            continue;
        }

        // token is an operator, parenthesis or brace
        if (OPERATOR_LOOKUP.count(token)) {
            while (!operator_stack.empty() && operator_stack.top() != "(" &&
                   PRIORITY_LOOKUP.at(operator_stack.top()) >
                       PRIORITY_LOOKUP.at(token)) {
                output_queue->push(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.push(token);
            continue;
        }

        // token is a number
        try {
            double num = std::stod(token);
            Matrix number_in_matrix(num, _factory);
            std::string new_name = TMP_NAME;
            new_name += std::to_string(variables.size());
            variables.emplace(new_name, number_in_matrix);
            output_queue->push(new_name);

        } catch (std::exception & e) {
            throw std::invalid_argument("Unknown token " + token);
        }
    }

    while (!operator_stack.empty()) {
        if (operator_stack.top() == "(" || operator_stack.top() == ")") {
            throw std::runtime_error("Parenthesis mismatch on input");
        }
        output_queue->push(operator_stack.top());
        operator_stack.pop();
    }

    return output_queue;
}

Matrix Parser::load_matrix(std::istream & stream) const {
    char c;
    std::vector<std::vector<double>> mx;
    while(stream.peek() != ']' && !stream.eof()){
        std::vector<double> row;
        stream >> c;
        if (c != '['){
            throw std::runtime_error("Matrix parse error.");
        }
        while (c != ']'){
            double val;
            stream >> val;
            row.emplace_back(val);
            stream >> c;
            if (c == ',' || c == ']'){
                continue;
            }
            throw std::runtime_error("Matrix parse error.");
        }

        if (!mx.empty() && mx.back().size() != row.size()){
            throw std::runtime_error("Matrix parse error.");
        }

        stream >> c;
        if (c == ',' || c == ']'){
            mx.emplace_back(row);
            continue;
        }
        throw std::runtime_error("Matrix parse error.");
    }

    MatrixDimensions dims(mx.size(), mx.at(0).size());

    return {
        {new DenseMatrixIterator(&dims, mx, 0, 0)},
        {new DenseMatrixIterator(&dims, mx, dims.rows(), 0)},
        _factory
    };
}
