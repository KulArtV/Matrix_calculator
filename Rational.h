#ifndef MATRIX_RATIONAL_H
#define MATRIX_RATIONAL_H

#include <exception>
#include <iostream>

class RationalDivisionByZero : public std::exception {
  const char* what() const noexcept override {
    return "You tried to divide by zero";
  }
};

class Rational {
 public:
  Rational(const long long p, const long long q) : p_(p), q_(q) {
    this->reduce();
  }
  explicit Rational(const long long p) : p_(p), q_(1) {}
  Rational() : p_(0), q_(1) {}

  long long getNumerator() const { return p_; }
  long long getDenominator() const { return q_; }

  friend std::istream& operator>>(std::istream& is, Rational& number);
  friend std::ostream& operator<<(std::ostream& os, const Rational& number);

  friend bool operator<(const Rational& rOne, const Rational& rTwo);
  friend bool operator>(const Rational& rOne, const Rational& rTwo);
  friend bool operator==(const Rational& rOne, const Rational& rTwo);
  friend bool operator>=(const Rational& rOne, const Rational& rTwo);
  friend bool operator<=(const Rational& rOne, const Rational& rTwo);
  friend bool operator!=(const Rational& rOne, const Rational& rTwo);

  friend Rational operator+(const Rational& rOne, const Rational& rTwo);
  friend Rational operator-(const Rational& rOne, const Rational& rTwo);
  friend Rational operator*(const Rational& rOne, const Rational& rTwo);
  friend Rational operator/(const Rational& rOne, const Rational& rTwo);

  Rational operator-() const { return Rational(-1 * p_, q_); }
  Rational operator+() const { return *this; }

  const Rational operator++(int);
  Rational& operator++() { return (*this += Rational(1)); }
  const Rational operator--(int);
  Rational& operator--() { return (*this -= Rational(1)); }

  Rational& operator+=(const Rational& number);
  Rational& operator-=(const Rational& number);
  Rational& operator*=(const Rational& number);
  Rational& operator/=(const Rational& number);

 private:
  long long p_, q_;

  static int gcd(int a, int b);
  void reduce();
};

#endif
