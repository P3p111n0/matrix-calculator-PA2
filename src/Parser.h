#pragma once

#include "Matrix.h"
#include "MatrixFactory.h"
#include <iostream>
#include <cstdlib>
#include <queue>
#include <string>
#include <unordered_map>

class Parser {
  public:
    Parser(std::istream &, std::size_t, const MatrixFactory &);
    std::shared_ptr<std::queue<std::string>> parse_input(std::unordered_map<std::string, Matrix> &) const;
  private:
    Matrix load_matrix(std::istream &) const;

    std::istream & _stream;
    std::size_t _max_len;
    MatrixFactory _factory;
};
