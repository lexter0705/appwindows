#include "finder.h"

#include <windows.h>

#include <memory>

#include "../core/window.h"
#include "exceptions/invalid_size.h"
#include "window.h"

namespace appwindows {
namespace windows {

FinderWindows::FinderWindows() = default;

std::vector<std::string> FinderWindows::get_all_titles() const {
  std::vector<std::string> titles;
  for (const auto& i : get_all_windows()) titles.push_back(*i->get_title());
  return titles;
}

std::shared_ptr<core::Window> FinderWindows::get_window_by_title(
    const std::string title) const {
  const auto windows = get_all_windows();
  for (auto window : windows)
    if (window->get_title()->find(title) != std::string::npos) return window;
  throw exceptions::WindowDoesNotExistException();
}

std::shared_ptr<core::Window> FinderWindows::get_window_by_process_id(
    const int process_id) const {
  const auto windows = get_all_windows();
  for (auto window : windows)
    if (*window->get_process_id() == process_id) return window;
  throw exceptions::WindowDoesNotExistException();
}

std::vector<std::shared_ptr<core::Window>> FinderWindows::get_all_windows()
    const {
  std::vector<std::shared_ptr<core::Window>> result;
  EnumWindows(
      [](const HWND hwnd, const LPARAM lparam) {
        auto& windows =
            *reinterpret_cast<std::vector<std::shared_ptr<core::Window>>*>(
                lparam);
        if (IsWindowVisible(hwnd))
          windows.push_back(
              std::make_shared<WindowWindows>(std::make_shared<HWND>(hwnd)));
        return TRUE;
      },
      reinterpret_cast<LPARAM>(&result));
  return result;
}

}  // namespace windows
}  // namespace appwindows