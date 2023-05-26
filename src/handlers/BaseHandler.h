#pragma once

#include "../matrix_wrapper/MatrixFactory.h"

/**
 * @brief A base class for handling I/O operations.
 */
class BaseHandler {
  public:

    /**
     * @brief Initializes the handler.
     * @param factory Factory to be used for matrix handling, like creating or
     *                exporting.
     */
    BaseHandler(MatrixFactory factory);
  protected:

    /**
     * @brief Factory used for creating and exporting matrices.
     */
    MatrixFactory _factory;
};
