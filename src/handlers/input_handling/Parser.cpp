#include "Parser.h"
#include "../../iterators/DenseMatrixIterator.h"
#include "../../matrix_operations/OperationFactory.h"
#include "InputHandler.h"
#include "ParsedInput.h"
#include <memory>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

inline constexpr char TMP_NAME[] = "TMP";

Parser::Parser(MatrixFactory factory, std::istream & stream,
               std::size_t max_input_len)
    : InputHandler(factory), _stream(stream), _max_len(max_input_len) {}

ParsedInput Parser::parse_input() const {
    ParsedInput result;
    OperationFactory operations;
    auto & output_queue = *result.output_queue;
    auto & variables = *result.loaded_variables;

    std::stack<std::string> operator_stack;

    std::unique_ptr<char[]> buffer(new char[_max_len]);
    _stream.getline(buffer.get(), _max_len - 1);

    std::stringstream line_stream(buffer.get());
    line_stream.peek();
    if (line_stream.eof()) {
        throw std::runtime_error("Parse error: Couldn't read data during parsing.");
    }
    while (line_stream >> std::ws && !line_stream.eof()) {
        std::string token;

        // inline matrix in input
        if (line_stream.peek() == '[') {
            char c;
            line_stream >> c; // this is the peeked '['
            std::string new_name = get_temporary_name(TMP_NAME);
            variables.emplace(new_name, load_matrix(line_stream));
            output_queue.push(new_name);
            continue;
        }

        line_stream >> token;
        if (string_has_prefix(token, RESERVED_NAME_PREFIX)) {
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

        if (token == "SCAN") {
            std::string name;
            if (!(line_stream >> name) ||
                string_has_prefix(name, RESERVED_NAME_PREFIX) ||
                operations.is_operation(token)) {
                throw std::invalid_argument(
                    "Invalid argument in call of SCAN.");
            }
            variables.emplace(name, load_matrix_scan(_stream));
            output_queue.emplace(name);
            break;
        }
        if (operations.is_operation(token)) {
            std::unique_ptr<MatrixOp> token_op(operations.get_operation(token));
            while (!operator_stack.empty() && operator_stack.top() != "(" &&
                   operations.priority_of(operator_stack.top()) >
                       token_op->priority()) {
                output_queue.push(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.push(token);
            continue;
        }

        // token is a number
        try {
            double num = std::stod(token);
            Matrix number_in_matrix(num);
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

        if (bracket != '[') {
            throw std::runtime_error(
                "Missing opening brace in scanned matrix.");
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