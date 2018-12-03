#ifndef MATRIX_BASE_H
#define MATRIX_BASE_H

#include "access-type.h"
#include "matrix-accessor.h"

template <typename T>
class MatrixBase
{

public:
  MatrixBase(unsigned int rows, unsigned int cols, AccessType accessType = ROW_MAJOR);

  virtual ~MatrixBase();

  virtual T get(unsigned int rowNum, unsigned int colNum) const;

  virtual unsigned getRows() const;

  virtual unsigned getCols() const;

  virtual void set(unsigned row, unsigned col, T val);

  virtual void display(unsigned startRow = 0, unsigned startCol = 0) const;

protected:
  unsigned rows, columns;
  IMatrixAccessor<T> *accessor;
};

#include "row-major-accessor.h"
#include "col-major-accessor.h"
#include <iostream>

template <typename T>
MatrixBase<T>::MatrixBase(unsigned rows, unsigned cols, AccessType accessType)
    : rows(rows), columns(cols)
{
  if (COLUMN_MAJOR == accessType)
  {
    accessor = new ColMajorAccessor<T>(rows, cols);
  }
  else
  {
    accessor = new RowMajorAccessor<T>(rows, cols);
  }
}

template <typename T>
MatrixBase<T>::~MatrixBase()
{
  delete accessor;
}

template <typename T>
unsigned MatrixBase<T>::getRows() const
{
  return rows;
}

template <typename T>
unsigned MatrixBase<T>::getCols() const
{
  return columns;
}

template <typename T>
T MatrixBase<T>::get(unsigned row, unsigned col) const
{
  return accessor->get(row, col);
}

template <typename T>
void MatrixBase<T>::set(unsigned row, unsigned col, T val)
{
  accessor->set(row, col, val);
}

template <typename T>
void MatrixBase<T>::display(unsigned startRow, unsigned startCol) const
{
  for (unsigned int i = startRow; i < rows; ++i)
  {
    for (unsigned int j = startCol; j < columns; ++j)
    {
      std::cout << accessor->get(i, j) << " ";
    }
    std::cout << std::endl;
  }
}
#endif