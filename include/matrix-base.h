#ifndef MATRIX_BASE_H
#define MATRIX_BASE_H

#include "access-type.h"
#include "logger.h"

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

  virtual void display(unsigned startRow, unsigned startCol, AccessType access) const;

  virtual AccessType getAccessType() const;

protected:
  unsigned rows, columns;
  T **data;
  AccessType accessType;
};

#include <iostream>

template <typename T>
MatrixBase<T>::MatrixBase(unsigned rows, unsigned cols, AccessType accessType)
    : rows(rows), columns(cols), accessType(accessType)
{
  ILOG << "Created Matrix of size [" << rows << ", " << cols << "]";
  data = new T *[rows]();
  for (unsigned int i = 0; i < rows; ++i)
  {
    data[i] = new T[cols]();
  }
}

template <typename T>
MatrixBase<T>::~MatrixBase()
{
  for (unsigned i = 0; i < rows; ++i)
  {
    delete[] data[i];
  }

  delete[] data;
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
  DLOG << "Getting element at [" << row << ", " << col << "]";
  if (row < rows && col < columns)
  {
    return data[row][col];
  }

  ELOG << "Invalid index specified: [" << row << ", "
       << col << "] for matrix of size [" << rows << ", " << columns << "]";
  return UINT32_MAX;
}

template <typename T>
void MatrixBase<T>::set(unsigned row, unsigned col, T val)
{
  DLOG << "Setting element at [" << row << ", " << col << "]";
  if (row < rows && col < columns)
  {
    data[row][col] = val;
  }
  else
  {
    ELOG << "Invalid index specified: [" << row << ", "
         << col << "] for matrix of size [" << rows << ", " << columns << "]";
  }
}

template <typename T>
void MatrixBase<T>::display(unsigned startRow, unsigned startCol, AccessType access) const
{
  ILOG << "Display matrix of size [" << rows << ", "
       << columns << "] from row [" << startRow << ", "
       << startCol << "] in [" << access << "] order";

  if (startRow >= rows || startCol >= columns)
  {
    ELOG << "Invalid indices specified!";
    return;
  }

  if (ROW_MAJOR == access)
  {
    for (unsigned i = startRow; i < rows; ++i)
    {
      for (unsigned j = startCol; j < columns; ++j)
      {
        std::cout << get(i, j) << " ";
      }
      std::cout << std::endl;
    }
  }
  else
  {
    for (unsigned j = startCol; j < columns; ++j)
    {
      for (unsigned i = startRow; i < rows; ++i)
      {
        std::cout << get(i, j) << " ";
      }
      std::cout << std::endl;
    }
  }
}

template <typename T>
AccessType MatrixBase<T>::getAccessType() const
{
  return accessType;
}

#endif