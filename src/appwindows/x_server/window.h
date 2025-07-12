#pragma once
#ifdef __linux__

#include <X11/Xlib.h>

#include <memory>
#include <vector>

#include "../core/geometry/point.h"
#include "../core/geometry/size.h"
#include "../core/window.h"

namespace appwindows {
namespace x_server {

class WindowXServer final : public core::WindowI {
 public:
  WindowXServer(Window window, Display* display);
  std::unique_ptr<std::vector<core::Point>> get_points() override;
  std::unique_ptr<core::Size> get_size() const override;
  std::unique_ptr<std::string> get_title() const override;

  void set_active(bool active) override;
  void set_maximize(bool is_maximize) override;
  void set_size(core::Size size) override;
  void move(core::Point point) override;
  void close() override;

 private:
  Display* display_;
  Window window_;
};

}  // namespace x_server
}  // namespace appwindows

#endif