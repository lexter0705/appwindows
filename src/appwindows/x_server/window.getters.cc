#ifdef __linux__

#include "window.h"

#include <X11/Xlib.h>

#include <memory>
#include <string>
#include <vector>

#include "../core/geometry/point.h"
#include "../core/geometry/size.h"

namespace appwindows {
namespace x_server {
std::unique_ptr<std::vector<core::Point>> WindowXServer::get_points() {
  XWindowAttributes attrs;
  if (!XGetWindowAttributes(display_, window_, &attrs)) return nullptr;
  auto points = std::make_unique<std::vector<core::Point>>();
  points->push_back({attrs.x, attrs.y});
  points->push_back({attrs.x + attrs.width, attrs.y});
  points->push_back({attrs.x + attrs.width, attrs.y + attrs.height});
  points->push_back({attrs.x, attrs.y + attrs.height});
  return points;
}

std::unique_ptr<core::Size> WindowXServer::get_size() const {
  XWindowAttributes attrs;
  if (!XGetWindowAttributes(display_, window_, &attrs)) return nullptr;
  return std::make_unique<core::Size>(attrs.width, attrs.height);
}

std::unique_ptr<std::string> WindowXServer::get_title() const {
  char* name = nullptr;
  if (XFetchName(display_, window_, &name) && name != nullptr) {
    auto title = std::make_unique<std::string>(name);
    XFree(name);
    return title;
  }
  return nullptr;
}

}  // namespace x_server
}  // namespace appwindows

#endif
