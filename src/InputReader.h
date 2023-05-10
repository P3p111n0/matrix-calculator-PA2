#pragma once

#include "Matrix.h"
#include "MatrixFactory.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <unordered_map>

class InputReader {
    InputReader(std::istream &, std::size_t, const MatrixFactory &);

    bool read_input(std::unordered_map<std::string, Matrix> &) const;
  private:
    bool parse_input(std::unordered_map<std::string, Matrix> &) const;
    bool evaluate_input(std::unordered_map<std::string, Matrix> &) const;
    Matrix load_matrix() const;

    std::istream & _stream;
    std::size_t _max_len;
    MatrixFactory _factory;
};
