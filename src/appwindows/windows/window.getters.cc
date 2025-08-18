#include "window.h"

#include <windows.h>

namespace appwindows {
namespace windows {

std::unique_ptr<std::string> WindowWindows::get_title() const {
  const int length = GetWindowTextLengthW(*window_);
  if (length == 0) return std::make_unique<std::string>("");
  std::wstring wide_title(length + 1, L'\0');
  GetWindowTextW(*window_, &wide_title[0], length + 1);
  const int utf8_size = WideCharToMultiByte(CP_UTF8, 0, wide_title.c_str(), -1,
                                            nullptr, 0, nullptr, nullptr);
  if (utf8_size == 0) return std::make_unique<std::string>("");
  std::string utf8_title(utf8_size, '\0');
  WideCharToMultiByte(CP_UTF8, 0, wide_title.c_str(), -1, &utf8_title[0],
                      utf8_size, nullptr, nullptr);
  utf8_title.resize(utf8_size - 1);
  return std::make_unique<std::string>(std::move(utf8_title));
}

std::unique_ptr<std::vector<core::Point>> WindowWindows::get_points() {
  RECT rect;
  GetWindowRect(*window_, &rect);
  auto points = std::make_unique<std::vector<core::Point>>();
  points->reserve(4);
  points->emplace_back(static_cast<int>(rect.left), static_cast<int>(rect.top));
  points->emplace_back(static_cast<int>(rect.right),
                       static_cast<int>(rect.top));
  points->emplace_back(static_cast<int>(rect.right),
                       static_cast<int>(rect.bottom));
  points->emplace_back(static_cast<int>(rect.left),
                       static_cast<int>(rect.bottom));
  return points;
}

std::unique_ptr<core::Size> WindowWindows::get_size() const {
  RECT rect;
  GetWindowRect(*window_, &rect);
  return std::make_unique<core::Size>(rect.right - rect.left,
                                      rect.bottom - rect.top);
}

std::shared_ptr<HWND> WindowWindows::get_window() const { return window_; }

py::array_t<unsigned char> WindowWindows::get_screenshot() const {
  throw std::runtime_error("Not implemented");
};

}  // namespace windows
}  // namespace appwindows
