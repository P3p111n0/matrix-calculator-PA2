#pragma once

#include "../handlers/config_handling/Configurator.h"
#include "../handlers/input_handling/Evaluator.h"
#include "../handlers/input_handling/Parser.h"
#include <iostream>

class MatrixCalculator {
  public:
    MatrixCalculator(std::istream &, std::ostream &, const std::string &);
    void start();
  private:
    Configurator _config;
    std::istream & _in;
    std::ostream & _out;
};
