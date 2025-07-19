#ifdef __linux__
#include "finder.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <memory>
#include <stdexcept>
#include <string>

#include "window.h"

namespace appwindows {
namespace x_server {

FinderXServer::FinderXServer() {
  display_ = XOpenDisplay(nullptr);
  if (!display_) throw std::runtime_error("Cannot open X11 display_");
}

std::unique_ptr<core::WindowI> FinderXServer::get_window_by_title(
    const std::string title) const override {
  Window root = DefaultRootWindow(display_);
  Window* children;
  unsigned int count_of_children = 0;
  XQueryTree(display_, root, &root, &root, &children, &count_of_children);
  if (count_of_children == 0) return nullptr;
  for (unsigned int i = 0; i < count_of_children; i++) {
    std::unique_ptr<core::WindowI> window = check_window(title, &children[i]);
    if (&window != nullptr) return window;
  }
  return nullptr;
}

std::unique_ptr<core::WindowI> FinderXServer::check_window(
    const std::string& title, Window* window) const {
  XTextProperty text_prop;
  if (XGetWMName(display_, *window, &text_prop))
    if (text_prop.value && text_prop.nitems > 0) {
      const std::string window_title(reinterpret_cast<char*>(text_prop.value));
      XFree(text_prop.value);
      if (window_title.find(title) != std::string::npos)
        return std::make_unique<WindowXServer>(*window, display_);
    }
  return nullptr;
}

}  // namespace x_server
}  // namespace appwindows

#endif