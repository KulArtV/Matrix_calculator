#include "Rational.h"

void Rational::reduce() {
  if (q_ < 0) {
    q_ *= -1;
    p_ *= -1;
  }
  if (p_ == 0) {
    q_ = 1;
  }
  int reduceNumber = gcd(abs(p_), abs(q_));
  p_ /= reduceNumber;
  q_ /= reduceNumber;
}

int Rational::gcd(int a, int b) {
  while (a > 0 && b > 0) {
    if (a > b) {
      a = a % b;
    } else {
      b = b % a;
    }
  }
  return (a + b);
}

std::istream& operator>>(std::istream& is, Rational& number) {
  int read = scanf("%lld/%lld", &number.p_, &number.q_);
  if (number.q_ == 0 || number.p_ == 0 || read < 2) {
    number.q_ = 1;
  }
  number.reduce();
  return is;
}
std::ostream& operator<<(std::ostream& os, const Rational& number) {
  if (number.p_ == 0) {
    std::cout << 0;
  } else if (number.q_ != 1) {
    std::cout << number.p_ << '/' << number.q_;
  } else {
    std::cout << number.p_;
  }
  return os;
}

bool operator<(const Rational& rOne, const Rational& rTwo) {
  return rOne.p_ * rTwo.q_ < rTwo.p_ * rOne.q_;
}
bool operator>(const Rational& rOne, const Rational& rTwo) {
  return rOne.p_ * rTwo.q_ > rTwo.p_ * rOne.q_;
}
bool operator==(const Rational& rOne, const Rational& rTwo) {
  return rOne.p_ * rTwo.q_ == rTwo.p_ * rOne.q_;
}
bool operator>=(const Rational& rOne, const Rational& rTwo) {
  return rOne.p_ * rTwo.q_ >= rTwo.p_ * rOne.q_;
}
bool operator<=(const Rational& rOne, const Rational& rTwo) {
  return rOne.p_ * rTwo.q_ <= rTwo.p_ * rOne.q_;
}
bool operator!=(const Rational& rOne, const Rational& rTwo) {
  return rOne.p_ * rTwo.q_ != rTwo.p_ * rOne.q_;
}

Rational operator+(const Rational& rOne, const Rational& rTwo) {
  Rational result(rOne.p_ * rTwo.q_ + rOne.q_ * rTwo.p_, rOne.q_ * rTwo.q_);
  result.reduce();
  return result;
}
Rational operator-(const Rational& rOne, const Rational& rTwo) {
  Rational result(rOne.p_ * rTwo.q_ - rOne.q_ * rTwo.p_, rOne.q_ * rTwo.q_);
  result.reduce();
  return result;
}
Rational operator*(const Rational& rOne, const Rational& rTwo) {
  return {rOne.p_ * rTwo.p_, rOne.q_ * rTwo.q_};
}
Rational operator/(const Rational& rOne, const Rational& rTwo) {
  if (rTwo == Rational(0)) {
    throw RationalDivisionByZero();
  }
  return {rOne.p_ * rTwo.q_, rOne.q_ * rTwo.p_};
}

const Rational Rational::operator++(int) {
  this->p_ += q_;
  return Rational(this->p_ - this->q_, this->q_);
}

const Rational Rational::operator--(int) {
  this->p_ -= q_;
  return Rational(this->p_ + this->q_, this->q_);
}

Rational& Rational::operator+=(const Rational& number) {
  return *this = (*this + number);
}
Rational& Rational::operator-=(const Rational& number) {
  return *this = (*this - number);
}
Rational& Rational::operator*=(const Rational& number) {
  return *this = (*this * number);
}
Rational& Rational::operator/=(const Rational& number) {
  return *this = (*this / number);
}