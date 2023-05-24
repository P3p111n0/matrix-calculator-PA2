#include "Parser.h"
#include "../../iterators/DenseMatrixIterator.h"
#include "../../matrix_wrapper/Matrix.h"
#include "../../matrix_wrapper/MatrixDimensions.h"
#include "../../matrix_wrapper/MatrixFactory.h"
#include "InputHandler.h"
#include "Lexer.h"
#include "ParsedInput.h"
#include <sstream>
#include <stack>
#include <string>
#include <vector>

inline constexpr char TMP_NAME[] = "TMP";

Parser::Parser(MatrixFactory factory, std::istream & stream, std::size_t max_input_len)
    : InputHandler(factory), _stream(stream), _max_len(max_input_len) {}

ParsedInput Parser::parse_input() const {
    ParsedInput result;

    auto & output_queue = *result.output_queue;
    auto & variables = *result.loaded_variables;

    std::stack<std::string> operator_stack;
    std::string line;

    std::getline(_stream, line);
    if (line.length() > _max_len) {
        throw std::runtime_error("Max input length exceeded");
    }

    std::stringstream line_stream(line);
    while (line_stream >> std::ws && !line_stream.eof()) {
        std::string token;

        // inline matrix in input
        if (line_stream.peek() == '[') {
            char c;
            line_stream >> c;
            Matrix parsed_matrix = load_matrix(line_stream);
            std::string new_name = get_temporary_name(TMP_NAME);
            variables.emplace(new_name, parsed_matrix);
            output_queue.push(new_name);
            continue;
        }

        line_stream >> token;
        if (string_has_prefix(token, RESERVED_NAME_PREFIX)){
            throw std::runtime_error("Token " + token + "is reserved.");
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
                output_queue.push(operator_stack.top());
                operator_stack.pop();
                if (operator_stack.empty()) {
                    throw std::runtime_error("Parenthesis mismatch on input");
                }
            }

            if (!operator_stack.empty() && operator_stack.top() != "(") {
                throw std::runtime_error("Parenthesis mismatch on input");
            }
            operator_stack.pop();
            continue;
        }

        // token is an operator, parenthesis or brace
        if (Lexer::OPERATOR_LOOKUP.count(token)) {
            if (token == "SCAN"){
                std::string name;
                if (!(line_stream >> name) || string_has_prefix(name, RESERVED_NAME_PREFIX)){
                    throw std::invalid_argument("Invalid argument in call of SCAN.");
                }
                auto scanned_mx = load_matrix_scan(_stream);
                variables.emplace(name, scanned_mx);
                output_queue.emplace(name);
                continue;
            }

            while (!operator_stack.empty() && operator_stack.top() != "(" &&
                   Lexer::PRIORITY_LOOKUP.at(operator_stack.top()) >
                       Lexer::PRIORITY_LOOKUP.at(token)) {
                output_queue.push(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.push(token);
            continue;
        }

        // token is a number
        try {
            double num = std::stod(token);
            Matrix number_in_matrix(num, _factory);
            std::string new_name = get_temporary_name(TMP_NAME);
            variables.emplace(new_name, number_in_matrix);
            output_queue.push(new_name);
        } catch (std::exception & e) {
            output_queue.push(token);
        }
    }

    while (!operator_stack.empty()) {
        if (operator_stack.top() == "(" || operator_stack.top() == ")") {
            throw std::runtime_error("Parenthesis mismatch on input");
        }
        output_queue.push(operator_stack.top());
        operator_stack.pop();
    }

    return result;
}

static bool read_row(std::istream & stream, std::vector<double> & row) {
    char c = 0;
    while (c != ']') {
        double val;
        stream >> val;
        row.emplace_back(val);
        stream >> c;
        switch (c) {
        case ',':
            continue;
        case ']':
            return true;
        default:
            return false;
        }
    }
    return false;
}

static inline Matrix
create_matrix_from_vec(const std::vector<std::vector<double>> & vec,
                       const MatrixFactory factory) {
    MatrixDimensions dims(vec.size(), vec.at(0).size());

    return {{new DenseMatrixIterator(&dims, vec, 0, 0)},
            {new DenseMatrixIterator(&dims, vec, dims.rows(), 0)},
            factory};
}

Matrix Parser::load_matrix(std::istream & stream) const {
    char c;
    std::vector<std::vector<double>> mx;
    while (stream.peek() != ']' && !stream.eof()) {
        std::vector<double> row;
        stream >> c;
        if (c != '[') {
            throw std::runtime_error("Matrix parse error.");
        }

        if (!read_row(stream, row)) {
            throw std::runtime_error("Matrix parse error.");
        }

        if (!mx.empty() && mx.back().size() != row.size()) {
            throw std::runtime_error("Matrix parse error.");
        }

        stream >> c;
        if (c == ',' || c == ']') {
            mx.emplace_back(row);
            if (c == ']') {
                break;
            }
            continue;
        }
        throw std::runtime_error("Matrix parse error.");
    }

    return create_matrix_from_vec(mx, _factory);
}

Matrix Parser::load_matrix_scan(std::istream & stream) const {
    std::vector<std::vector<double>> mx;
    std::string line;

    while (std::getline(stream, line) && !line.empty()) {
        std::vector<double> row;
        std::istringstream line_stream(line);

        char bracket;
        line_stream >> bracket;

        if (bracket != '['){
            throw std::runtime_error("Missing opening brace in scanned matrix.");
        }

        if (!read_row(line_stream, row)) {
            throw std::runtime_error("Matrix scan error.");
        }
        if (!mx.empty() && mx.back().size() != row.size()) {
            throw std::runtime_error("Row length mismatch in matrix.");
        }
        mx.emplace_back(row);
    }

    return create_matrix_from_vec(mx, _factory);
}