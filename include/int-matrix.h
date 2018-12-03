#ifndef INT_MATRIX_H
#define INT_MATRIX_H

#include "matrix-base.h"

class IntMatrix : public MatrixBase<int>
{
  public:
    IntMatrix(unsigned int rows, unsigned int columns, AccessType accessType = ROW_MAJOR);

    ~IntMatrix();
};

#endif