#pragma once
#ifdef __linux__

#include <X11/Xlib.h>

#include "../core/finder.h"

namespace appwindows {
namespace x_server {

class FinderXServer final : public core::FinderI {
 public:
  FinderXServer();
  std::unique_ptr<core::WindowI> get_window_by_title(
      std::string title) const override;
  std::unique_ptr<core::WindowI> check_window(const std::string& title,
                                              Window* window) const;

 private:
  std::unique_ptr<core::WindowI> FinderXServer::check_window(
      const std::string& title, Window* window);
  Display* display_;
};

}  // namespace x_server
}  // namespace appwindows

#endif