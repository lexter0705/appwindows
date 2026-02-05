#pragma once

#include <pybind11/numpy.h>

#import <AppKit/AppKit.h>
#import <Accessibility/Accessibility.h>
#include <CoreGraphics/CGWindow.h>

#include "../core/core.h"

namespace appwindows::macos {

class WindowMacOS final : public core::Window {
public:
  explicit WindowMacOS(AXUIElementRef window_ref);
  ~WindowMacOS();

  WindowMacOS(const WindowMacOS&) = delete;
  WindowMacOS& operator=(const WindowMacOS&) = delete;

  [[nodiscard]] std::unique_ptr<core::QuadPoints> get_points() override;
  [[nodiscard]] std::unique_ptr<std::string> get_title() const override;
  [[nodiscard]] std::unique_ptr<core::Size> get_size() const override;
  [[nodiscard]] std::unique_ptr<core::Size> get_min_size() const override;
  [[nodiscard]] std::unique_ptr<core::Size> get_max_size() const override;
  [[nodiscard]] py::array_t<unsigned char> get_screenshot() override;
  [[nodiscard]] std::unique_ptr<int> get_process_id() const override;
  [[nodiscard]] std::unique_ptr<bool> is_valid() const override;
  AXUIElementRef get_window_ref() const;

  void set_minimize(bool is_minimize) override;
  void set_fullscreen(bool is_fullscreen) override;
  void resize(core::Size size) override;
  void move(core::Point point) override;
  void close() override;
  void to_foreground() override;
  void to_background() override;

private:
  AXUIElementRef window_ref_;
  void handle_error(AXError error);
};

}  // namespace appwindows::macos