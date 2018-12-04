#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include "matrices.h"
#include <ostream>

enum MatrixType
{
    INT = 1,
    LONG,
    FLOAT,
    DOUBLE
};

inline std::ostream &operator<<(std::ostream &os, const MatrixType &matrix)
{
    switch (matrix)
    {
    case INT:
        os << "INT";
        break;
    case LONG:
        os << "LONG";
        break;
    case FLOAT:
        os << "FLOAT";
        break;
    case DOUBLE:
        os << "DOUBLE";
        break;
    }

    return os;
}

class MatrixOperations
{
  public:
    MatrixOperations();
    ~MatrixOperations();

    void CreateMatrix();

  private:
    MatrixType matrixType;
    IntMatrix *intMatrix;
    LongMatrix *longMatrix;
    FloatMatrix *floatMatrix;
    DoubleMatrix *doubleMatrix;
};

#endif