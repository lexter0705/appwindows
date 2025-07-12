#pragma once

namespace appwindows {
namespace core {

class Size {
 public:
  Size(int width, int height);
  [[nodiscard]] int getWidth() const noexcept { return width_; };
  [[nodiscard]] int getHeight() const noexcept { return height_; };

 private:
  int width_;
  int height_;
};

}  // namespace core
}  // namespace appwindows
