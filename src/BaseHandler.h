#pragma once

#include "MatrixFactory.h"
class BaseHandler {
  public:
    BaseHandler(MatrixFactory);
  protected:
    MatrixFactory _factory;
};
