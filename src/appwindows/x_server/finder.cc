#ifdef __linux__
#include "window.getters.h"  // Заголовочный файл для WindowXServer

namespace appwindows {
namespace x_server {

FinderXServer::FinderXServer() {
  display_ = XOpenDisplay(nullptr);
  if (!display_) throw std::runtime_error("Cannot open X11 display_");
}

std::unique_ptr<core::WindowI> FinderXServer::get_window_by_title(
    const std::string title) const override {
  if (!display_) return nullptr;
  Window root = DefaultRootWindow(display_);
  Window* children;
  unsigned int count_of_children;
  if (!XQueryTree(display_, root, &root, &root, &children, &count_of_children))
    return nullptr;
  std::unique_ptr<WindowXServer> result;
  for (unsigned int i = 0; i < count_of_children; i++) {
    XTextProperty text_prop;
    if (XGetWMName(display_, children[i], &text_prop)) {
      if (text_prop.value && text_prop.nitems > 0) {
        std::string window_title(reinterpret_cast<char*>(text_prop.value));
        XFree(text_prop.value);
        if (window_title.find(title) != std::string::npos) {
          result = std::make_unique<WindowXServer>(children[i], display_);
          break;
        }
      }
    }
  }
  if (children) XFree(children);
  return result;
}

}  // namespace x_server
}  // namespace appwindows

#endif