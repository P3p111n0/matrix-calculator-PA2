#pragma once

#include <cstdlib>
#include <iostream>

class Configurator {
  public:
    explicit Configurator(std::ostream &);

    void load_config(const char *);

    double sparse_ratio;
    std::size_t max_input_length;
  private:
    std::ostream & _stream;

    void print_defaults(std::ostream &) const;
    void set_defaults();
};

