#pragma once

#include "MatrixElement.h"

#ifndef MATRIXCALCULATOR_ABSTRACTMATRIXITERATOR_H
#define MATRIXCALCULATOR_ABSTRACTMATRIXITERATOR_H

class AbstractMatrixIterator {
  public:
    virtual void operator++() = 0;
    virtual MatrixElement operator*() const = 0;
};

#endif // MATRIXCALCULATOR_ABSTRACTMATRIXITERATOR_H
