#pragma once

#include <memory>
#include <string>

#include "../core/finder.h"
#include "../core/window.h"

namespace appwindows {
namespace windows {

class FinderWindows final : public core::Finder {
 public:
  FinderWindows();
  [[nodiscard]] std::shared_ptr<core::Window> get_window_by_title(
      std::string title) const override;
  [[nodiscard]] std::vector<std::shared_ptr<core::Window>> get_all_windows()
      const override;
  [[nodiscard]] std::shared_ptr<core::Window> open_new_window(
      std::string path_to_file, int sleep_time) const override;
};

}  // namespace windows
}  // namespace appwindows
