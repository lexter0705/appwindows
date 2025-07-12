#ifdef __linux__

#include "window.h"

#include <X11/Xlib.h>

#include "../core/geometry/point.h"
#include "../core/geometry/size.h"

namespace appwindows {
namespace x_server {

WindowXServer::WindowXServer(const Window window, Display* display)
    : display_(display), window_(window) {}

void WindowXServer::set_active(bool active) {
  if (active) {
    XSetInputFocus(display_, window_, RevertToParent, CurrentTime);
    XRaiseWindow(display_, window_);
  } else {
    Window root = DefaultRootWindow(display_);
    XSetInputFocus(display_, root, RevertToParent, CurrentTime);
  }
  XFlush(display_);
}

void WindowXServer::set_maximize(const bool is_maximize) {
  Atom wm_state = XInternAtom(display_, "_NET_WM_STATE", False);
  Atom max_horz = XInternAtom(display_, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
  Atom max_vert = XInternAtom(display_, "_NET_WM_STATE_MAXIMIZED_VERT", False);
  if (wm_state == None || max_horz == None || max_vert == None) return;
  XEvent event;
  memset(&event, 0, sizeof(event));
  event.type = ClientMessage;
  event.xclient.window = window_;
  event.xclient.message_type = wm_state;
  event.xclient.format = 32;
  event.xclient.data.l[0] = is_maximize ? 1 : 0;
  event.xclient.data.l[1] = max_horz;
  event.xclient.data.l[2] = max_vert;
  event.xclient.data.l[3] = 1;
  XSendEvent(display_, DefaultRootWindow(display_), False,
             SubstructureRedirectMask | SubstructureNotifyMask, &event);
  XFlush(display_);
}

void WindowXServer::set_size(const core::Size size) {
  XResizeWindow(display_, window_, size.getWidth(), size.getHeight());
  XFlush(display_);
}

void WindowXServer::move(const core::Point point) {
  XMoveWindow(display_, window_, point.getX(), point.getY());
  XFlush(display_);
}

void WindowXServer::close() {
  Atom wm_protocols = XInternAtom(display_, "WM_PROTOCOLS", False);
  Atom wm_delete = XInternAtom(display_, "WM_DELETE_WINDOW", False);
  if (wm_protocols != None && wm_delete != None) {
    XEvent event;
    memset(&event, 0, sizeof(event));
    event.type = ClientMessage;
    event.xclient.window = window_;
    event.xclient.message_type = wm_protocols;
    event.xclient.format = 32;
    event.xclient.data.l[0] = wm_delete;
    event.xclient.data.l[1] = CurrentTime;
    XSendEvent(display_, window_, False, NoEventMask, &event);
  } else {
    XDestroyWindow(display_, window_);
  }
  XFlush(display_);
}

}  // namespace x_server
}  // namespace appwindows

#endif