#pragma once

#include "InputHandler.h"
#include "Matrix.h"
#include "MatrixFactory.h"
#include <cstdlib>
#include <iostream>
#include <queue>
#include <unordered_map>

class Parser : public InputHandler {
  public:
    Parser(MatrixFactory, std::istream &, std::size_t);
    std::shared_ptr<std::queue<std::string>>
    parse_input(std::unordered_map<std::string, Matrix> &) const;

  private:
    Matrix load_matrix(std::istream &) const;
    Matrix load_matrix_scan(std::istream &) const;

    std::istream & _stream;
    std::size_t _max_len;
};
