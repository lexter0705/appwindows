#pragma once

#include <pybind11/numpy.h>
#include <windows.h>

#include "../core/geometry/point.h"
#include "../core/geometry/size.h"
#include "../core/window.h"

namespace appwindows {
namespace macos {

class WindowMacos final : public core::Window {
 public:
  WindowMacos();
  std::unique_ptr<std::vector<core::Point>> get_points() override;
  [[nodiscard]] std::unique_ptr<std::string> get_title() const override;
  [[nodiscard]] std::unique_ptr<core::Size> get_size() const override;
  [[nodiscard]] py::array_t<unsigned char> get_screenshot() override;

  void set_minimize(bool is_minimize) override;
  void set_fullscreen(bool is_fullscreen) override;
  void resize(core::Size size) override;
  void move(core::Point point) override;
  void close() override;
  void to_foreground() override;
  void to_background() override;
};

}  // namespace macos
}  // namespace appwindows
