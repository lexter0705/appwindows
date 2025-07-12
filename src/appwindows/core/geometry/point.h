#pragma once

namespace appwindows {
namespace core {

class Point {
 public:
  Point(int x, int y);
  Point operator+(const Point& other) const noexcept;
  Point operator-(const Point& other) const noexcept;
  Point operator*(const Point& other) const noexcept;
  Point operator/(const Point& other) const;
  [[nodiscard]] int getX() const noexcept { return x_; };
  [[nodiscard]] int getY() const noexcept { return y_; };

 private:
  int x_;
  int y_;
};

}  // namespace core
}  // namespace appwindows