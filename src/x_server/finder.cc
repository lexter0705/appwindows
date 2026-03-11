#include "finder.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <memory>
#include <stdexcept>
#include <string>

#include "../core/base_window.h"
#include "window.h"

using WindowX = Window;

namespace appwindows::x_server {

FinderXServer::FinderXServer() = default;

Display* FinderXServer::open_display() {
  auto display = XOpenDisplay(nullptr);
  if (!display) throw std::runtime_error("Cannot open X11 display");
  return display;
}

std::unique_ptr<std::string> FinderXServer::get_os() const {
  return std::make_unique<std::string>("Linux::X11");
}

std::vector<std::shared_ptr<core::Window>> FinderXServer::get_all_windows()
    const {
  Display* display = open_display();
  WindowX root = DefaultRootWindow(display);

  Atom net_wm_window_type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
  Atom net_wm_window_type_normal =
      XInternAtom(display, "_NET_WM_WINDOW_TYPE_NORMAL", False);
  Atom xa_atom = XInternAtom(display, "ATOM", False);

  std::vector<std::shared_ptr<core::Window>> windows;

  collect_windows(display, root, net_wm_window_type,
                 net_wm_window_type_normal, xa_atom, windows);

  XCloseDisplay(display);
  return windows;
}

bool FinderXServer::is_normal_window(Display* display, WindowX window,
                    Atom net_wm_window_type, Atom net_wm_window_type_normal,
                    Atom xa_atom) const {
  Atom type;
  int format;
  unsigned long nitems, bytes_after;
  unsigned char* data = nullptr;
  if (XGetWindowProperty(display, window, net_wm_window_type, 0, ~0L,
                         False, xa_atom, &type, &format, &nitems,
                         &bytes_after, &data) != Success) return false;
  bool is_normal = false;
  if (type == xa_atom && data) {
    Atom* types = reinterpret_cast<Atom*>(data);
    for (unsigned long j = 0; j < nitems; ++j) {
      if (types[j] == net_wm_window_type_normal) {
        is_normal = true;
        break;
      }
    }
  }
  if (data) XFree(data);
  return is_normal;
}

void FinderXServer::collect_windows(Display* display, WindowX root,
                    Atom net_wm_window_type, Atom net_wm_window_type_normal,
                    Atom xa_atom,
                    std::vector<std::shared_ptr<core::Window>>& out) const {
  WindowX unused_root, unused_parent;
  WindowX* children = nullptr;
  unsigned int nchildren = 0;
  if (!XQueryTree(display, root, &unused_root, &unused_parent, 
      &children, &nchildren)) return;
  for (unsigned int i = 0; i < nchildren; ++i) {
    WindowX child = children[i];
    if (is_normal_window(display, child, net_wm_window_type,
                       net_wm_window_type_normal, xa_atom))
      out.push_back(std::make_shared<WindowXServer>(child));
    collect_windows(display, child, net_wm_window_type,
                   net_wm_window_type_normal, xa_atom, out);
  }
  if (children) XFree(children);
}

}  // namespace appwindows::x_server