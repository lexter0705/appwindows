#ifdef _WIN32 || _WIN64

#include "window.h"

#include <windows.h>

#include "../core/instruction.h"

namespace appwindows {
namespace windows {

std::unique_ptr<std::string> WindowWindows::get_title() const {
  char buffer[256];
  const int result = GetWindowTextA(*window_, buffer, sizeof(buffer));
  return std::make_unique<std::string>(result > 0 ? buffer : "");
}

std::unique_ptr<std::vector<core::Point>> WindowWindows::get_points() {
  RECT rect;
  GetWindowRect(*window_, &rect);
  return std::make_unique<std::vector<core::Point>>{
      core::Point{static_cast<int>(rect.left), static_cast<int>(rect.top)},
      core::Point{static_cast<int>(rect.right), static_cast<int>(rect.top)},
      core::Point{static_cast<int>(rect.right), static_cast<int>(rect.bottom)},
      core::Point{static_cast<int>(rect.left), static_cast<int>(rect.bottom)},
  };
}

std::unique_ptr<core::Size> WindowWindows::get_size() const {
  RECT rect;
  GetWindowRect(*window_, &rect);
  return std::make_unique<core::Size>(rect.right - rect.left,
                                      rect.bottom - rect.top);
}

std::shared_ptr<HWND> WindowWindows::get_window() const { return window_; }

}  // namespace windows
}  // namespace appwindows

#endif