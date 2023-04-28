#pragma once

#include "AbstractMatrixIterator.h"
#include <memory>

#ifndef MATRIXCALCULATOR_ITERATORWRAPPER_H
#define MATRIXCALCULATOR_ITERATORWRAPPER_H

class IteratorWrapper {
  public:
    IteratorWrapper(AbstractMatrixIterator *);
    void operator++();
    MatrixElement operator*() const;
    std::size_t operator-(const IteratorWrapper &) const;
    bool operator==(const IteratorWrapper &) const;
    bool operator!=(const IteratorWrapper &) const;
  private:
    std::unique_ptr<AbstractMatrixIterator> _iterator;
};

#endif // MATRIXCALCULATOR_ITERATORWRAPPER_H
