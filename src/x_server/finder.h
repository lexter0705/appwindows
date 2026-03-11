#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <memory>
#include <vector>

#include "../core/core.h"

using WindowX = Window;

namespace appwindows::x_server {

class FinderXServer final : public core::Finder {
 public:
  FinderXServer();
  [[nodiscard]]
  std::vector<std::shared_ptr<core::Window>> get_all_windows() const override;
  [[nodiscard]]
  std::unique_ptr<std::string> get_os() const override;
  static Display* open_display();

 private:
  bool is_normal_window(Display* display, WindowX window,
                    Atom net_wm_window_type, Atom net_wm_window_type_normal,
                    Atom xa_atom) const;
  void collect_windows(Display* display, WindowX root,
                    Atom net_wm_window_type, Atom net_wm_window_type_normal,
                    Atom xa_atom,
                    std::vector<std::shared_ptr<core::Window>>& out) const;
};

}  // namespace appwindows::x_server