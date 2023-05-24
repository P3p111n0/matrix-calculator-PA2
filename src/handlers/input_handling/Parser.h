#pragma once

#include "../../matrix_wrapper/Matrix.h"
#include "../../matrix_wrapper/MatrixFactory.h"
#include "InputHandler.h"
#include "ParsedInput.h"
#include <cstdlib>
#include <iostream>

class Parser : public InputHandler {
  public:
    Parser(MatrixFactory, std::istream &, std::size_t);
    ParsedInput parse_input() const;

  private:
    Matrix load_matrix(std::istream &) const;
    Matrix load_matrix_scan(std::istream &) const;

    std::istream & _stream;
    std::size_t _max_len;
};
