#pragma once

#include "../matrix_wrapper/MatrixFactory.h"
class BaseHandler {
  public:
    BaseHandler(MatrixFactory);
  protected:
    MatrixFactory _factory;
};
