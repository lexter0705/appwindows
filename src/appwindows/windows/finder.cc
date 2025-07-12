#ifdef _WIN32 || _WIN64

#include "finder.h"

#include <windows.h>

#include <memory>

#include "../core/window.h"
#include "window.h"

namespace appwindows {
namespace windows {

std::unique_ptr<core::WindowI> FinderWindows::get_window_by_title(
    const std::string title) const override {
  auto window = std::make_shared<HWND>(FindWindowA(nullptr, title.c_str()));
  if (!window) return nullptr;
  return std::make_unique<WindowWindows>(window);
}

}  // namespace windows
}  // namespace appwindows

#endif