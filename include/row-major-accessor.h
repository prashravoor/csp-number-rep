#ifndef ROW_MAJOR_DISPLAY_H
#define ROW_MAJOR_DISPLAY_H

#include "matrix-accessor.h"
#include "matrix-base.h"

template <typename T>
class RowMajorAccessor : public IMatrixAccessor<T>
{
public:
  RowMajorAccessor(unsigned rows, unsigned cols);

  ~RowMajorAccessor();

  T get(unsigned row, unsigned col) const;

  void set(unsigned row, unsigned col, T val);

  void seek(unsigned row, unsigned col);

  bool hasNext() const;

  T next();

private:
  T **data;
  unsigned rows, cols, rowIndex, colIndex;
};

#include <iostream>

template <typename T>
RowMajorAccessor<T>::RowMajorAccessor(unsigned rows, unsigned cols) : rows(rows),
                                                                      cols(cols),
                                                                      rowIndex(0),
                                                                      colIndex(0)
{
  data = new T *[rows]();
  for (unsigned int i = 0; i < rows; ++i)
  {
    data[i] = new T[cols]();
  }
}

template <typename T>
RowMajorAccessor<T>::~RowMajorAccessor()
{
  for (unsigned int i = 0; i < rows; ++i)
  {
    delete[] data[i];
  }

  delete[] data;
}

template <typename T>
T RowMajorAccessor<T>::get(unsigned row, unsigned col) const
{
  if (row < rows && col < cols)
  {
    return data[row][col];
  }

  return UINT32_MAX;
}

template <typename T>
void RowMajorAccessor<T>::set(unsigned row, unsigned col, T val)
{
  if (row < rows && col < cols)
  {
    data[row][col] = val;
  }
}

template <typename T>
void RowMajorAccessor<T>::seek(unsigned row, unsigned col)
{
  rowIndex = row;
  colIndex = col;
}

template <typename T>
bool RowMajorAccessor<T>::hasNext() const
{
  return (rowIndex < rows) && (colIndex < cols);
}

template <typename T>
T RowMajorAccessor<T>::next()
{
  if (rowIndex >= rows || colIndex >= cols)
  {
    return UINT32_MAX;
  }

  T tmp = data[rowIndex][colIndex];
  if (colIndex == cols - 1)
  {
    rowIndex++;
  }

  colIndex = (colIndex + 1) % cols;
  return tmp;
}
#endif