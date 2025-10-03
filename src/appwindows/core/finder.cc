#pragma once

#include "finder.h"

#include "exceptions/window_does_not_exist.h"

namespace appwindows {
namespace core {

std::vector<std::string> Finder::get_all_titles() const {
  std::vector<std::string> titles;
  for (const auto& i : get_all_windows()) titles.push_back(*i->get_title());
  return titles;
}

std::shared_ptr<Window> Finder::get_window_by_title(
    const std::string title) const {
  const auto windows = Finder::get_all_windows();
  for (auto window : windows)
    if (window->get_title()->find(title) != std::string::npos) return window;
  throw exceptions::WindowDoesNotExistException();
}

std::shared_ptr<Window> Finder::get_window_by_process_id(
    const int process_id) const {
  const auto windows = Finder::get_all_windows();
  for (auto window : windows)
    if (*window->get_process_id() == process_id) return window;
  throw exceptions::WindowDoesNotExistException();
}

}  // namespace core
}  // namespace appwindows