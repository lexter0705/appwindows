#pragma once

#ifdef _WIN32 || _WIN64

#include <windows.h>

#include "../core/geometry/point.h"
#include "../core/geometry/size.h"
#include "../core/window.h"

namespace appwindows {
namespace windows {

class WindowWindows final : public core::Window {
 public:
  explicit WindowWindows(const std::shared_ptr<HWND>& window);
  bool window_is_valid() const;
  std::unique_ptr<std::vector<core::Point>> get_points() override;
  [[nodiscard]] std::unique_ptr<std::string> get_title() const override;
  [[nodiscard]] std::unique_ptr<core::Size> get_size() const override;
  [[nodiscard]] std::shared_ptr<HWND> get_window() const;

  void set_active(bool active) override;
  void set_maximize(bool is_maximize) override;
  void set_size(core::Size size) override;
  void move(core::Point point) override;
  void close() override;

 private:
  std::shared_ptr<HWND> window_;
};

}  // namespace windows
}  // namespace appwindows

#endif