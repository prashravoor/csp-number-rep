#ifndef COL_MAJOR_DISPLAY_H
#define COL_MAJOR_DISPLAY_H

#include "matrix-accessor.h"
#include "matrix-base.h"
#include "logger.h"

template <typename T>
class ColMajorAccessor : public IMatrixAccessor<T>
{
public:
  ColMajorAccessor(unsigned rows, unsigned cols);

  ~ColMajorAccessor();

  AccessType getAccessType() const;

  T get(unsigned col, unsigned row) const;

  void set(unsigned row, unsigned col, T val);

  void seek(unsigned row, unsigned col);

  bool hasNext() const;

  T next();

private:
  T **data;
  unsigned rows, cols;
  unsigned rowIndex, colIndex;
};

#include <iostream>

template <typename T>
ColMajorAccessor<T>::ColMajorAccessor(unsigned rows, unsigned cols) : rows(rows),
                                                                      cols(cols),
                                                                      rowIndex(0),
                                                                      colIndex(0)
{
  ILOG << "Created Column Major Matrix of size [" << rows << ", " << cols << "]";
  data = new T *[cols]();
  for (unsigned int i = 0; i < cols; ++i)
  {
    data[i] = new T[rows]();
  }
}

template <typename T>
ColMajorAccessor<T>::~ColMajorAccessor()
{
  DLOG << "Deleting Column Major matrix";
  for (unsigned int i = 0; i < cols; ++i)
  {
    delete[] data[i];
  }

  delete[] data;
}

template <typename T>
AccessType ColMajorAccessor<T>::getAccessType() const
{
  return COLUMN_MAJOR;
}

template <typename T>
T ColMajorAccessor<T>::get(unsigned row, unsigned col) const
{
  DLOG << "Getting element [" << row << ", " << col << "] in column major order";
  if (row < cols && col < rows)
  {
    return data[col][row];
  }

  ELOG << "Failed to find element at position [" << row << ", " << col
       << "] in Column Major matrix of size [" << rows << ", " << cols << "]";
  return UINT32_MAX;
}

template <typename T>
void ColMajorAccessor<T>::set(unsigned row, unsigned col, T val)
{
  DLOG << "Attempting to set element [" << row << ", " << col
       << "] in Column Major matrix to value [" << val << "]";
  if (row < cols && col < rows)
  {
    data[col][row] = val;
  }
  else
  {
    ELOG << "Failed to find element at position [" << row << ", " << col
         << "] in matrix of size [" << rows << ", " << cols << "]";
  }
}

template <typename T>
void ColMajorAccessor<T>::seek(unsigned row, unsigned col)
{
  rowIndex = col;
  colIndex = row;
}

template <typename T>
bool ColMajorAccessor<T>::hasNext() const
{
  return (rowIndex < rows) && (colIndex < cols);
}

template <typename T>
T ColMajorAccessor<T>::next()
{
  DLOG << "Current Row = " << rowIndex << ", Col = " << colIndex;
  if (rowIndex >= rows || colIndex >= cols)
  {
    return UINT32_MAX;
  }

  T tmp = get(colIndex, rowIndex);
  DLOG << "Data[" << colIndex << ", " << rowIndex << "] = " << tmp;
  if (colIndex == cols - 1)
  {
    rowIndex++;
    std::cout << std::endl;
  }

  colIndex = (colIndex + 1) % cols;
  DLOG << "Current Row = " << rowIndex << ", Col = " << colIndex;
  return tmp;
}

#endif