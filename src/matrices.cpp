#include "matrices.h"

IntMatrix::IntMatrix(unsigned rows, unsigned cols, AccessType accessType) : MatrixBase(rows, cols, accessType)
{
}

IntMatrix::~IntMatrix()
{
}

LongMatrix::LongMatrix(unsigned rows, unsigned cols, AccessType accessType) : MatrixBase(rows, cols, accessType)
{
}

LongMatrix::~LongMatrix()
{
}

FloatMatrix::FloatMatrix(unsigned rows, unsigned cols, AccessType accessType) : MatrixBase(rows, cols, accessType)
{
}

FloatMatrix::~FloatMatrix()
{
}

DoubleMatrix::DoubleMatrix(unsigned rows, unsigned cols, AccessType accessType) : MatrixBase(rows, cols, accessType)
{
}

DoubleMatrix::~DoubleMatrix()
{
}