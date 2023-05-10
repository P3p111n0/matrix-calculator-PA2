#pragma once

#include <cstdlib>
#include <iostream>

class Configurator {
  public:
    Configurator(std::ostream *);

    void load_config(const char *);
  private:
    double _ratio;
    std::size_t _max_len;
    std::ostream * _stream;

    void error_and_reset(std::ostream &);
    void print_defaults(std::ostream &) const;
    void syntax_error(std::ostream &) const;
    void set_defaults();
};
