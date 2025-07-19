#ifdef _WIN32 || _WIN64

#include "window.h"

#include <windows.h>

#include "../core/exceptions/window_does_not_exist.h"
#include "../core/geometry/point.h"
#include "../core/geometry/size.h"

namespace appwindows {
namespace windows {

WindowWindows::WindowWindows(const std::shared_ptr<HWND> &window)
    : window_(window) {}

bool WindowWindows::window_is_valid() const {
  return IsWindow(*window_) != FALSE;
}

void WindowWindows::set_active(const bool active) {
  if (!window_is_valid()) throw core::exceptions::WindowDoesNotExistException();

  if (active) {
    SetForegroundWindow(*window_);
    SetActiveWindow(*window_);
    SetFocus(*window_);
  } else {
    HWND hwnd = GetNextWindow(*window_, GW_HWNDNEXT);
    if (hwnd) {
      SetForegroundWindow(hwnd);
      SetActiveWindow(hwnd);
    }
  }
}

void WindowWindows::set_maximize(const bool is_maximize) {
  if (!window_is_valid()) throw core::exceptions::WindowDoesNotExistException();
  ShowWindow(*window_, is_maximize ? SW_MAXIMIZE : SW_RESTORE);
}

void WindowWindows::set_size(const core::Size size) {
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

}  // namespace windows
}  // namespace appwindows

#endif