#pragma once
#include <ostream>
#include <cassert>

namespace mesh_tools {
class Point2 {
public:

  Point2() {
    data_[0] = data_[1] = 0;
  }

  Point2(const double x, const double y) {
    data_[0] = x;
    data_[1] = y;
  }

  Point2(const Point2& from) {
    data_[0] = from[0];
    data_[1] = from[1];
  }

  Point2(Point2&& from) noexcept {
    data_[0] = from[0];
    data_[1] = from[1];
  }

  ~Point2() = default;

  Point2& operator=(const Point2& from) {
    data_[0] = from[0];
    data_[1] = from[1];
    return *this;
  }

  Point2& operator=(Point2&& from) noexcept {
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

  Point2 operator+(const Point2& another_point) const {
    return {data_[0] + another_point[0], data_[1] + another_point[1]};
  }

  Point2 operator-(const Point2& another_point) const {
    return {data_[0] - another_point[0], data_[1] - another_point[1]};
  }

  Point2 operator*(const double number) const {
    return {data_[0] * number, data_[1] * number};
  }

  Point2 operator/(const double number) const {
    return {data_[0] / number, data_[1] / number};
  }

  double inner_product(const Point2& another_point) const {
    return data_[0] * another_point[0] + data_[1] * another_point[1];
  }

  double length2() const {
    return data_[0] * data_[0] + data_[1] * data_[1];
  }

  double length() const {
    return std::sqrt(length2());
  }

  double distance_to(const Point2& another_point) const {
    return (*this - another_point).length();
  }

  double distance2_to(const Point2& another_point) const {
    return (*this - another_point).length2();
  }

  friend Point2 operator*(const double number, const Point2& point) {
    return {point[0] * number, point[1] * number};
  }


  friend std::ostream& operator<<(std::ostream& out, const Point2& point) {
    out << "(" << point[0] << ", " << point[1] << ")";
    return out;
  }

private:
  double data_[2]{};
};

}
