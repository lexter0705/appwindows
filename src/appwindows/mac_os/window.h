#pragma once

#include "../core/geometry/point.h"
#include "../core/geometry/size.h"
#include "../core/window.h"

namespace appwindows {
namespace windows {

class WindowMacOs final : public core::Window {
 public:
  explicit WindowMacOs();
  std::unique_ptr<std::vector<core::Point>> get_points() override;
  [[nodiscard]] std::unique_ptr<std::string> get_title() const override;
  [[nodiscard]] std::unique_ptr<core::Size> get_size() const override;

  void set_active(bool active) override;
  void set_maximize(bool is_maximize) override;
  void set_size(core::Size size) override;
  void move(core::Point point) override;
  void close() override;
};

}  // namespace windows
}  // namespace appwindows
