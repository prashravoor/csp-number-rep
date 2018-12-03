#ifndef MATRIX_ACCESSOR_H
#define MATRIX_ACCESSOR_H

template <typename T>
class IMatrixAccessor
{
public:
  virtual T get(unsigned row, unsigned col) const = 0;

  virtual void set(unsigned row, unsigned col, T val) = 0;

  virtual void seek(unsigned row, unsigned col) = 0;

  virtual bool hasNext() const = 0;

  virtual T next() = 0;

  virtual ~IMatrixAccessor();
};

template <typename T>
IMatrixAccessor<T>::~IMatrixAccessor()
{
}

#endif