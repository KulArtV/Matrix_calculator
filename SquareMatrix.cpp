#include "Matrix.cpp"

class MatrixIsDegenerateError : public std::exception {
  const char* what() const noexcept override {
    return "Matrix's determinant is zero - it's bad idea to invert it";
  }
};

template <typename T>
class SquareMatrix : public Matrix<T> {
 public:
  explicit SquareMatrix<T>(const Matrix<T>& other);
  explicit SquareMatrix<T>(int size);
  SquareMatrix<T>& operator=(const SquareMatrix<T>& other) {
    if (other.matrixField_ != this->matrixField_) {
      this->ClearMatrix();
      this->width_ = other.width_;
      this->height_ = other.height_;
      this->matrixField_ = new T*[this->height_];
      for (int i = 0; i < this->height_; ++i) {
        this->matrixField_[i] = new T[this->width_];
      }
      for (int i = 0; i < this->height_; ++i) {
        for (int j = 0; j < this->width_; ++j) {
          this->matrixField_[i][j] = other(i, j);
        }
      }
    }
    return *this;
  }

  T getTrace() const;
  T getDeterminant() const;

  int getSize() const;

  SquareMatrix& invert();
  SquareMatrix getInverse() const;

  SquareMatrix& Transpose();
  SquareMatrix getTransposed();

  template <typename M>
  friend SquareMatrix<M> operator+(const SquareMatrix<M>& lmx,
                                   const SquareMatrix<M>& rmx);
  template <typename M>
  friend SquareMatrix<M> operator-(const SquareMatrix<M>& lmx,
                                   const SquareMatrix<M>& rmx);
  template <typename M>
  friend SquareMatrix<M> operator*(const SquareMatrix<M>& lmx,
                                   const SquareMatrix<M>& rmx);

  template <typename M, typename U>
  friend SquareMatrix<M> operator*(const U& scalar, const SquareMatrix<M>& rmx);
  template <typename M, typename U>
  friend SquareMatrix<M> operator*(const SquareMatrix<M>& lmx, const U& scalar);

  template <typename M, typename U>
  friend Matrix<M> operator*(const SquareMatrix<M>& lmx, const Matrix<U>& rmx);
  template <typename M, typename U>
  friend Matrix<M> operator*(const Matrix<M>& lmx, const SquareMatrix<U>& rmx);

  template <typename U>
  friend void GaussAlgorithm(SquareMatrix<U>& matrix, bool& isDetZero,
                             int& stringSwapsCounter);
  template <typename U, typename M>
  friend void CreateInvert(SquareMatrix<M>& ematrix, SquareMatrix<U>& matrix,
                           bool& isDetZero);
};

template <typename T>
int SquareMatrix<T>::getSize() const {
  return this->getColumnsNumber();
}

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::Transpose() {
  return (Matrix<T>(*this).Transpose());
}
template <typename T>
SquareMatrix<T> SquareMatrix<T>::getTransposed() {
  return static_cast<SquareMatrix<T>>(Matrix<T>(*this).Transpose());
}

template <typename T>
SquareMatrix<T>::SquareMatrix(const int size) {
  this->width_ = size;
  this->height_ = size;
  this->matrixField_ = new T*[size];
  for (int i = 0; i < size; ++i) {
    this->matrixField_[i] = new T[size];
  }
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      (*this)(i, j) = getZero<T>();
    }
  }
}
template <typename T>
SquareMatrix<T>::SquareMatrix(const Matrix<T>& other) {
  int size = other.getColumnsNumber();
  this->height_ = this->width_ = size;
  this->matrixField_ = new T*[size];
  for (int i = 0; i < size; ++i) {
    this->matrixField_[i] = new T[size];
  }
  for (int i = 0; i < this->width_; ++i) {
    for (int j = 0; j < this->width_; ++j) {
      (*this)(i, j) = other(i, j);
    }
  }
}

template <typename T>
T SquareMatrix<T>::getTrace() const {
  T trace = 0;
  for (int i = 0; i < this->width_; ++i) {
    trace += (*this)(i, i);
  }
  return trace;
}

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::invert() {
  bool isDetZero = false;
  SquareMatrix<T> matrix(*this);
  SquareMatrix<T> eMatrix(this->getSize());
  for (int i = 0; i < eMatrix.width_; ++i) {
    for (int j = 0; j < eMatrix.width_; ++j) {
      if (i == j) {
        eMatrix(i, j) = 1;
      } else {
        eMatrix(i, j) = 0;
      }
    }
  }
  CreateInvert(eMatrix, matrix, isDetZero);
  *this = eMatrix;
  if (isDetZero) {
    throw MatrixIsDegenerateError();
  } else {
    return *this;
  }
}
template <typename T>
SquareMatrix<T> SquareMatrix<T>::getInverse() const {
  bool isDetZero = false;
  SquareMatrix<T> matrix(*this);
  SquareMatrix<T> eMatrix(this->getSize());
  for (int i = 0; i < eMatrix.width_; ++i) {
    for (int j = 0; j < eMatrix.width_; ++j) {
      if (i == j) {
        eMatrix(i, j) = getOne<T>();
      } else {
        eMatrix(i, j) = getZero<T>();
      }
    }
  }
  CreateInvert(eMatrix, matrix, isDetZero);
  if (isDetZero) {
    throw MatrixIsDegenerateError();
  } else {
    return eMatrix;
  }
}

template <typename T>
T SquareMatrix<T>::getDeterminant() const {
  bool isDetZero = false;
  T Det = 1;
  SquareMatrix<T> matrix(*this);
  int stringSwapsCounter = 0;
  GaussAlgorithm(matrix, isDetZero, stringSwapsCounter);
  if (isDetZero) {
    return 0;
  } else {
    for (int i = 0; i < this->getSize(); ++i) {
      Det *= matrix(i, i);
    }
    return Det * (stringSwapsCounter % 2 == 0 ? 1 : -1);
  }
}

template <typename U, typename T>
void CreateInvert(SquareMatrix<T>& eMatrix, SquareMatrix<U>& matrix,
                  bool& isDetZero) {
  int size = matrix.getSize();

  bool isThereZeroSubMatrix;
  for (int i = 0; i < size; ++i) {
    if (matrix(i, i) != 0) {
      for (int j = i + 1; j < size; ++j) {
        if (matrix(j, i) != 0) {
          T temp = matrix(j, i) / matrix(i, i);
          for (int k = 0; k < size; ++k) {
            matrix(j, k) -= matrix(i, k) * temp;
            eMatrix(j, k) -= eMatrix(i, k) * temp;
          }
        }
      }
    } else {
      isThereZeroSubMatrix = true;
      for (int j = i + 1; j < size; ++j) {
        if (matrix(j, i) != 0) {
          std::swap(matrix.matrixField_[i], matrix.matrixField_[j]);
          std::swap(eMatrix.matrixField_[i], eMatrix.matrixField_[j]);
          isThereZeroSubMatrix = false;
          break;
        }
      }
      if (i < size - 1) {
        --i;
      }
      if (isThereZeroSubMatrix) {
        isDetZero = true;
        break;
      }
    }
  }

  if (!isDetZero) {
    for (int i = size - 1; i >= 0; --i) {
      for (int j = i - 1; j >= 0; --j) {
        T temp = matrix(j, i) / matrix(i, i);
        for (int k = size - 1; k >= 0; --k) {
          matrix(j, k) -= matrix(i, k) * temp;
          eMatrix(j, k) -= eMatrix(i, k) * temp;
        }
      }
    }

    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        eMatrix(i, j) /= matrix(i, i);
      }
      matrix(i, i) = 1;
    }
  }
}
template <typename T>
void GaussAlgorithm(SquareMatrix<T>& matrix, bool& isDetZero,
                    int& stringSwapsCounter) {
  int size = matrix.getSize();

  bool isThereZeroSubMatrix;
  for (int i = 0; i < size; ++i) {
    if (matrix(i, i) != 0) {
      for (int j = i + 1; j < size; ++j) {
        if (matrix(j, i) != 0) {
          T temp = matrix(j, i) / matrix(i, i);
          for (int k = i; k < size; ++k) {
            matrix(j, k) -= matrix(i, k) * temp;
          }
        }
      }
    } else {
      isThereZeroSubMatrix = true;
      for (int j = i + 1; j < size; ++j) {
        if (matrix(j, i) != 0) {
          std::swap(matrix.matrixField_[i], matrix.matrixField_[j]);
          ++stringSwapsCounter;
          isThereZeroSubMatrix = false;
          break;
        }
      }
      if (i < size - 1) {
        --i;
      }
      if (isThereZeroSubMatrix) {
        isDetZero = true;
        break;
      }
    }
  }
}

template <typename T>
SquareMatrix<T> operator+(const SquareMatrix<T>& lmx,
                          const SquareMatrix<T>& rmx) {
  return static_cast<SquareMatrix<T>>(Matrix<T>(lmx) + Matrix<T>(rmx));
}
template <typename T>
SquareMatrix<T> operator-(const SquareMatrix<T>& lmx,
                          const SquareMatrix<T>& rmx) {
  return static_cast<SquareMatrix<T>>(Matrix<T>(lmx) - Matrix<T>(rmx));
}
template <typename T>
SquareMatrix<T> operator*(const SquareMatrix<T>& lmx,
                          const SquareMatrix<T>& rmx) {
  return static_cast<SquareMatrix<T>>(Matrix<T>(lmx) * Matrix<T>(rmx));
}

template <typename T, typename M>
SquareMatrix<T> operator*(const SquareMatrix<T>& lmx, const M& value) {
  const Matrix<T>& a = lmx;
  return static_cast<SquareMatrix<T>>(a * value);
}
template <typename T, typename M>
SquareMatrix<T> operator*(const M& value, const SquareMatrix<T>& lmx) {
  const Matrix<T>& a = lmx;
  return static_cast<SquareMatrix<T>>(a * value);
}

template <typename T, typename M>
Matrix<T> operator*(const SquareMatrix<T>& lmx, const Matrix<M>& rmx) {
  return lmx * Matrix<M>(lmx);
}
template <typename T, typename M>
Matrix<T> operator*(const Matrix<T>& lmx, const SquareMatrix<M>& rmx) {
  return lmx * Matrix<M>(rmx);
}