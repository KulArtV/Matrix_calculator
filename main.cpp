#include <iostream>
#include "SquareMatrix.cpp"
#include "Rational.h"


int main() {
  int m, n, p;
  Rational r;
  std::cin >> m >> n >> p >> r;

  Matrix<Rational> A(m, n);
  SquareMatrix<Rational> S(p);
  std::cin >> A >> S;

  try {
    std::cout << (A * S) * A.getTransposed() << '\n';
  } catch (const MatrixWrongSizeError&) {
    std::cout << "A and S have not appropriate sizes for multiplication.\n";
  }

  std::cout << (r * (S = S) * S).getSize() << '\n';

  SquareMatrix<Rational> P(S);

  std::cout << (P * (S + S - 3 * P)).getDeterminant() << '\n';

  const SquareMatrix<Rational>& rS = S;

  std::cout << rS.getSize() << ' ' << rS.getDeterminant() << ' ' << rS.getTrace()
       << '\n';
  std::cout << (S = S) * (S + rS) << '\n';
  std::cout << (S *= S) << '\n';

  try {
    std::cout << rS.getInverse() << '\n';
    std::cout << P.invert().getTransposed().getDeterminant() << '\n';
    std::cout << P << '\n';
  } catch (const MatrixIsDegenerateError&) {
    std::cout << "Cannot inverse matrix." << '\n';
  }

  return 0;
}

