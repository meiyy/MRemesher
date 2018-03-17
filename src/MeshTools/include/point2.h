#pragma once
#include <ostream>
#include <cassert>

namespace mesh_tools {
class point2 {
public:

  point2() {
    data_[0] = data_[1] = 0;
  }

  point2(const double x, const double y) {
    data_[0] = x;
    data_[1] = y;
  }

  point2(const point2& from) {
    data_[0] = from[0];
    data_[1] = from[1];
  }

  point2(point2&& from) noexcept {
    data_[0] = from[0];
    data_[1] = from[1];
  }

  ~point2() = default;

  point2& operator=(const point2& from) {
    data_[0] = from[0];
    data_[1] = from[1];
    return *this;
  }

  point2& operator=(point2&& from) noexcept {
    data_[0] = from[0];
    data_[1] = from[1];
    return *this;
  }

  double& operator[](const int pos) {
    assert(pos >= 0 && pos < 2);
    return data_[pos];
  }

  const double& operator[](const int pos) const {
    assert(pos >= 0 && pos < 2);
    return data_[pos];
  }

  point2 operator+(const point2& another_point) const {
    return {data_[0] + another_point[0], data_[1] + another_point[1]};
  }

  point2 operator-(const point2& another_point) const {
    return {data_[0] - another_point[0], data_[1] - another_point[1]};
  }

  point2 operator*(const double number) const {
    return {data_[0] * number, data_[1] * number};
  }

  point2 operator/(const double number) const {
    return {data_[0] / number, data_[1] / number};
  }

  double inner_product(const point2& another_point) const {
    return data_[0] * another_point[0] + data_[1] * another_point[1];
  }

  double length2() const {
    return data_[0] * data_[0] + data_[1] * data_[1];
  }

  double length() const {
    return std::sqrt(length2());
  }

  double distance_to(const point2& another_point) const {
    return (*this - another_point).length();
  }

  double distance2_to(const point2& another_point) const {
    return (*this - another_point).length2();
  }

  friend point2 operator*(const double number, const point2& point) {
    return {point[0] * number, point[1] * number};
  }


  friend std::ostream& operator<<(std::ostream& out, const point2& point) {
    out << "(" << point[0] << ", " << point[1] << ")";
    return out;
  }

private:
  double data_[2]{};
};

}
