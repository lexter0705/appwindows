#ifdef _WIN32 || _WIN64

#include "window.h"

#include <windows.h>

#include <algorithm>

#include "../core/exceptions/window_does_not_exist.h"
#include "../core/geometry/point.h"
#include "../core/geometry/size.h"

namespace appwindows {
namespace windows {

WindowWindows::WindowWindows(const std::shared_ptr<HWND>& window)
    : window_(window) {}

bool WindowWindows::window_is_valid() const {
  return IsWindow(*window_) != FALSE;
}

void WindowWindows::set_active(const bool active) {
  if (!window_is_valid()) throw core::exceptions::WindowDoesNotExistException();
  if (active)
    SetWindowPos(*window_, HWND_TOP, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
  else
    SetWindowPos(*window_, HWND_TOP, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW | SW_RESTORE);
}

void WindowWindows::set_maximize(const bool is_maximize) {
  if (!window_is_valid()) throw core::exceptions::WindowDoesNotExistException();
  ShowWindow(*window_, is_maximize ? SW_MAXIMIZE : SW_RESTORE);
}

void WindowWindows::resize(const core::Size size) {
  if (!window_is_valid()) throw core::exceptions::WindowDoesNotExistException();
  RECT rect;
  GetWindowRect(*window_, &rect);
  SetWindowPos(*window_, nullptr, rect.left, rect.top, size.getWidth(),
               size.getHeight(), SWP_NOZORDER | SWP_NOACTIVATE);
}

void WindowWindows::move(const core::Point point) {
  if (!window_is_valid()) throw core::exceptions::WindowDoesNotExistException();
  const std::unique_ptr<core::Size> size = get_size();
  SetWindowPos(*window_, nullptr, point.getX(), point.getY(), size->getWidth(),
               size->getHeight(), SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

void WindowWindows::close() {
  if (!window_is_valid()) throw core::exceptions::WindowDoesNotExistException();
  PostMessage(*window_, WM_CLOSE, 0, 0);
}

void WindowWindows::set_layer(core::layers::Layer& layer) {
  if (!window_is_valid()) throw std::runtime_error("Invalid window handle");
  const DWORD current_pid = GetCurrentProcessId();
  std::vector<HWND> windows;
  HWND hwnd = nullptr;
  while ((hwnd = FindWindowEx(nullptr, hwnd, nullptr, nullptr)))
    if (IsWindowVisible(hwnd)) {
      DWORD pid;
      GetWindowThreadProcessId(hwnd, &pid);
      if (pid == current_pid) {
        windows.push_back(hwnd);
      }
    }
  const int layer_pos = layer.get_layer(static_cast<int>(windows.size()));
  if (!windows.empty()) {
    HWND insert_after = HWND_BOTTOM;
    if (layer_pos >= static_cast<int>(windows.size()) - 1) {
      insert_after = HWND_TOPMOST;
    } else if (layer_pos > 0) {
      if (const auto it = std::find(windows.begin(), windows.end(), *window_);
          it != windows.end() && it + 1 != windows.end()) {
        insert_after = *(it + 1);
      }
    }
    SetWindowPos(*window_, insert_after, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
  }
}

}  // namespace windows
}  // namespace appwindows

#endif