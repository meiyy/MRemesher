#pragma once
#include <ostream>
#include <cassert>

namespace mesh_tools {
class Point3 {
public:

  // Constractors, destructor, assignment operator
  Point3() {
    data_[0] = data_[1] = data_[2] = 0;
  }

  Point3(const double x, const double y, const double z) {
    data_[0] = x;
    data_[1] = y;
    data_[2] = z;
  }

  Point3(const Point3& from) {
    data_[0] = from[0];
    data_[1] = from[1];
    data_[2] = from[2];
  }

  Point3(Point3&& from) noexcept {
    data_[0] = from[0];
    data_[1] = from[1];
    data_[2] = from[2];
  }

  ~Point3() = default;


  friend Point3 operator*(const double number, const Point3& point) {
    return {point[0] * number, point[1] * number, point[2] * number};
  }


  friend std::ostream& operator<<(std::ostream& out, const Point3& point) {
    out << "(" << point[0] << ", " << point[1] << ", " << point[2] << ")";
    return out;
  }

  Point3& operator=(const Point3& from) {
    data_[0] = from[0];
    data_[1] = from[1];
    data_[2] = from[2];
    return *this;
  }

  Point3& operator=(Point3&& from) noexcept {
    data_[0] = from[0];
    data_[1] = from[1];
    data_[2] = from[2];
    return *this;
  }

  double& operator[](const int pos) {
    assert(pos >= 0 && pos < 3);
    return data_[pos];
  }

  const double& operator[](const int pos) const {
    assert(pos >= 0 && pos < 3);
    return data_[pos];
  }

  Point3 operator+(const Point3& another_point) const {
    return {
      data_[0] + another_point[0], data_[1] + another_point[1],
      data_[2] + another_point[2]
    };
  }

  Point3 operator-(const Point3& another_point) const {
    return {
      data_[0] - another_point[0], data_[1] - another_point[1],
      data_[2] - another_point[2]
    };
  }

  Point3 operator*(const double number) const {
    return {data_[0] * number, data_[1] * number, data_[2] * number};
  }

  Point3 operator/(const double number) const {
    return {data_[0] / number, data_[1] / number, data_[2] / number};
  }

  double inner_product(const Point3& another_point) const {
    return data_[0] * another_point[0] + data_[1] * another_point[1] + data_[2]
           *
           another_point[2];
  }

  Point3 outer_product(const Point3& another_point) const {
    return {
      data_[1] * another_point[2] - data_[2] * another_point[1],
      data_[2] * another_point[0] - data_[0] * another_point[2],
      data_[0] * another_point[1] - data_[1] * another_point[0]
    };
  }

  double length2() const {
    return data_[0] * data_[0] + data_[1] * data_[1] + data_[2] * data_[2];
  }

  double length() const {
    return std::sqrt(length2());
  }

  double distance_to(const Point3& another_point) const {
    return (*this - another_point).length();
  }

  double distance2_to(const Point3& another_point) const {
    return (*this - another_point).length2();
  }

  Point3 normalize() const {
    return *this / length();
  }

private:
  double data_[3]{};
};
}
