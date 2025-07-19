#include "point.h"

#include <stdexcept>

namespace appwindows {
namespace core {

Point::Point(const int x, const int y) : x_(x), y_(y) {}

Point Point::operator+(const Point& other) const{
  return Point{x_ + other.getX(), y_ + other.getY()};
}

Point Point::operator-(const Point& other) const{
  return Point{x_ - other.getX(), y_ - other.getY()};
}

Point Point::operator/(const Point& other) const {
  if (other.getX() == 0 || other.getY() == 0) {
    throw std::invalid_argument("Division by zero in Point coordinates");
  }
  return Point{x_ / other.getX(), y_ / other.getY()};
}

Point Point::operator*(const Point& other) const{
  return Point{x_ * other.getX(), y_ * other.getY()};
}

int Point::getX() const { return x_; }

int Point::getY() const { return y_; }

}  // namespace core
}  // namespace appwindows