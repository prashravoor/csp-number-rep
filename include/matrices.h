#ifndef INT_MATRIX_H
#define INT_MATRIX_H

#include "matrix-base.h"

class IntMatrix : public MatrixBase<int>
{
public:
  IntMatrix(unsigned int rows, unsigned int columns, AccessType accessType = ROW_MAJOR);

  ~IntMatrix();
};

class LongMatrix : public MatrixBase<long>
{
public:
  LongMatrix(unsigned rows, unsigned columns, AccessType accessType = ROW_MAJOR);

  ~LongMatrix();
};

class FloatMatrix : public MatrixBase<float>
{
public:
  FloatMatrix(unsigned rows, unsigned columns, AccessType accessType = ROW_MAJOR);

  ~FloatMatrix();
};

class DoubleMatrix : public MatrixBase<double>
{
public:
  DoubleMatrix(unsigned rows, unsigned columns, AccessType accessType = ROW_MAJOR);

  ~DoubleMatrix();
};

#endif