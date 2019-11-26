#include <exception>
#include <iostream>

template <typename T>
T getZero() {
  return T(0);
}

template <typename T>
T getOne() {
  return T(1);
}

class MatrixWrongSizeError : public std::exception {
  const char* what() const noexcept override {
    return "Matrix's sizes don't fit to make the operation";
  }
};

class MatrixIndexError : public std::exception {
  const char* what() const noexcept override {
    return "You tried to get an element with incorrect pair of index";
  }
};

template <typename T>
class Matrix {
 public:
  Matrix<T>() = default;
  Matrix(int height, int width);
  Matrix(const Matrix<T>& other);
  Matrix(Matrix<T>&& other) noexcept;
  Matrix& operator=(const Matrix& other);
  Matrix& operator=(Matrix&& other) noexcept;
  virtual ~Matrix();
  Matrix<T>& Transpose();
  Matrix<T> getTransposed();

  int getRowsNumber() const { return height_; }
  int getColumnsNumber() const { return width_; }

  template <typename M>
  friend Matrix<M> operator+(const Matrix<M>& lmx, const Matrix<M>& rmx);
  template <typename M>
  friend Matrix<M> operator-(const Matrix<M>& lmx, const Matrix<M>& rmx);
  template <typename M>
  friend Matrix<M> operator*(const Matrix<M>& lmx, const Matrix<M>& rmx);
  template <typename M, typename U>
  friend Matrix<M> operator*(const U& scalar, const Matrix<M>& rmx);
  template <typename M, typename U>
  friend Matrix<M> operator*(const Matrix<M>& rmx, const U& scalar);

  template <typename M>
  Matrix<T>& operator+=(const M& number) {
    return *this = (*this + number);
  }
  template <typename M>
  Matrix<T>& operator-=(const M& number) {
    return *this = (*this - number);
  }
  template <typename M>
  Matrix<T>& operator*=(const M& number) {
    return *this = (*this * number);
  }

  T& operator()(int positionHeight, int positionWidth);
  T operator()(int positionHeight, int positionWidth) const;

  template <typename M>
  friend std::istream& operator>>(std::istream& is, Matrix<M>& matrix);
  template <typename M>
  friend std::ostream& operator<<(std::ostream& os, const Matrix<M>& matrix);

  virtual void ClearMatrix() {
    for (int i = 0; i < height_; ++i) {
      delete[] matrixField_[i];
    }
    delete[] matrixField_;
  }

 protected:
  int height_ = 0;
  T** matrixField_;

  int width_ = 0;
};

template <typename T>
Matrix<T>::Matrix(const int height, const int width) {
  height_ = height;
  width_ = width;
  matrixField_ = new T*[height];
  for (int i = 0; i < height_; ++i) {
    matrixField_[i] = new T[width];
  }
  for (int i = 0; i < height_; ++i) {
    for (int j = 0; j < width_; ++j) {
      matrixField_[i][j] = 0;
    }
  }
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other) {
  width_ = other.width_;
  height_ = other.height_;
  matrixField_ = new T*[height_];
  for (int i = 0; i < height_; ++i) {
    matrixField_[i] = new T[width_];
  }
  for (int i = 0; i < height_; ++i) {
    for (int j = 0; j < width_; ++j) {
      matrixField_[i][j] = other.matrixField_[i][j];
    }
  }
}
template <typename T>
Matrix<T>::Matrix(Matrix<T>&& other) noexcept {
  std::swap(matrixField_, other.matrixField_);
  width_ = other.width_;
  height_ = other.height_;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
  if (other.matrixField_ != this->matrixField_) {
    this->ClearMatrix();
    width_ = other.width_;
    height_ = other.height_;
    this->matrixField_ = new T*[height_];
    for (int i = 0; i < height_; ++i) {
      matrixField_[i] = new T[width_];
    }
    for (int i = 0; i < height_; ++i) {
      for (int j = 0; j < width_; ++j) {
        matrixField_[i][j] = other.matrixField_[i][j];
      }
    }
  }
  return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other) noexcept {
  if (other.matrixField_ != this->matrixField_) {
    width_ = other.width_;
    height_ = other.height_;
    std::swap(matrixField_, other.matrixField_);
  }
  return *this;
}

template <typename T>
Matrix<T>::~Matrix() {
  for (int i = 0; i < height_; ++i) {
    delete[] matrixField_[i];
  }
  delete[] matrixField_;
}

template <typename T>
Matrix<T> operator+(const Matrix<T>& lmx, const Matrix<T>& rmx) {
  if (lmx.width_ != rmx.width_ || lmx.height_ != rmx.height_) {
    throw MatrixWrongSizeError();
  }
  Matrix<T> newMatrix(lmx.height_, lmx.width_);
  for (int i = 0; i < newMatrix.height_; ++i) {
    for (int j = 0; j < newMatrix.width_; ++j) {
      newMatrix.matrixField_[i][j] =
          lmx.matrixField_[i][j] + rmx.matrixField_[i][j];
    }
  }
  return newMatrix;
}
template <typename T>
Matrix<T> operator-(const Matrix<T>& lmx, const Matrix<T>& rmx) {
  if (lmx.width_ != rmx.width_ || lmx.height_ != rmx.height_) {
    throw MatrixWrongSizeError();
  }
  Matrix<T> newMatrix(lmx.height_, lmx.width_);
  for (int i = 0; i < newMatrix.height_; ++i) {
    for (int j = 0; j < newMatrix.width_; ++j) {
      newMatrix.matrixField_[i][j] =
          lmx.matrixField_[i][j] - rmx.matrixField_[i][j];
    }
  }
  return newMatrix;
}
template <typename T, typename U>
Matrix<T> operator*(const U& scalar, const Matrix<T>& lmx) {
  Matrix<T> newMatrix(lmx.height_, lmx.width_);
  for (int i = 0; i < newMatrix.height_; ++i) {
    for (int j = 0; j < newMatrix.width_; ++j) {
      newMatrix.matrixField_[i][j] = lmx.matrixField_[i][j] * scalar;
    }
  }
  return newMatrix;
}

template <typename T, typename U>
Matrix<T> operator*(const Matrix<T>& lmx, const U& scalar) {
  Matrix<T> newMatrix(lmx.height_, lmx.width_);
  for (int i = 0; i < newMatrix.height_; ++i) {
    for (int j = 0; j < newMatrix.width_; ++j) {
      newMatrix.matrixField_[i][j] = lmx.matrixField_[i][j] * scalar;
    }
  }
  return newMatrix;
}
template <typename T>
Matrix<T> operator*(const Matrix<T>& lmx, const Matrix<T>& rmx) {
  if (lmx.width_ != rmx.height_) {
    throw MatrixWrongSizeError();
  }
  Matrix<T> newMatrix(lmx.height_, rmx.width_);
  for (int i = 0; i < lmx.height_; ++i) {
    for (int j = 0; j < rmx.width_; ++j) {
      for (int k = 0; k < lmx.width_; ++k) {
        newMatrix.matrixField_[i][j] +=
            lmx.matrixField_[i][k] * rmx.matrixField_[k][j];
      }
    }
  }
  return newMatrix;
}

template <typename T>
T& Matrix<T>::operator()(const int positionHeight, const int positionWidth) {
  if (positionHeight < 0 || positionHeight >= height_ || positionWidth < 0 ||
      positionWidth >= width_) {
    throw MatrixIndexError();
  }
  return matrixField_[positionHeight][positionWidth];
}
template <typename T>
T Matrix<T>::operator()(const int positionHeight,
                        const int positionWidth) const {
  if (positionHeight < 0 || positionHeight >= height_ || positionWidth < 0 ||
      positionWidth >= width_) {
    throw MatrixIndexError();
  }
  return matrixField_[positionHeight][positionWidth];
}

template <typename T>
std::istream& operator>>(std::istream& is, Matrix<T>& matrix) {
  matrix.matrixField_ = new T*[matrix.height_];
  for (int i = 0; i < matrix.height_; ++i) {
    matrix.matrixField_[i] = new T[matrix.width_];
  }

  for (int i = 0; i < matrix.height_; ++i) {
    for (int j = 0; j < matrix.width_; ++j) {
      is >> matrix.matrixField_[i][j];
    }
  }
  return is;
}
template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) {
  for (int i = 0; i < matrix.height_; ++i) {
    for (int j = 0; j < matrix.width_; ++j) {
      os << matrix.matrixField_[i][j] << ' ';
    }
    os << '\n';
  }
  return os;
}

template <typename T>
Matrix<T> Matrix<T>::getTransposed() {
  Matrix<T> newMatrix(width_, height_);
  for (int i = 0; i < height_; ++i) {
    for (int j = 0; j < width_; ++j) {
      newMatrix.matrixField_[j][i] = matrixField_[i][j];
    }
  }
  return newMatrix;
}
template <typename T>
Matrix<T>& Matrix<T>::Transpose() {
  Matrix<T> newMatrix(width_, height_);
  for (int i = 0; i < height_; ++i) {
    for (int j = 0; j < width_; ++j) {
      newMatrix.matrixField_[j][i] = matrixField_[i][j];
    }
  }
  *this = newMatrix;
  return *this;
}
