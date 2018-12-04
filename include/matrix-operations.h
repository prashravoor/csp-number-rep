#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include "matrices.h"
#include <ostream>

#define MAX_COL_SIZE 4096
#define MAX_ROW_SIZE 4096

enum MatrixType
{
    NA,
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
    case NA:
        os << "NA";
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

    void ReadMatrix();

    void FreeMatrix();

    void ShowMatrix();

  private:
    void readIntMatrix(bool manual);
    void readLongMatrix(bool manual);
    void readDoubleMatrix(bool manual);
    void readFloatMatrix(bool manual);

    MatrixType matrixType;
    IntMatrix *intMatrix;
    LongMatrix *longMatrix;
    FloatMatrix *floatMatrix;
    DoubleMatrix *doubleMatrix;
};

#endif