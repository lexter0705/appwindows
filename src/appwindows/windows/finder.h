#pragma once

#include <memory>

#include "../core/finder.h"
#include "../core/window.h"

namespace appwindows {
namespace windows {

class FinderWindows final : public core::Finder {
 public:
  FinderWindows();
  [[nodiscard]]
  std::vector<std::shared_ptr<core::Window>> get_all_windows() const override;
  [[nodiscard]]
  std::shared_ptr<core::Window> get_window_by_process_id(int process_id) const override;
  [[nodiscard]]
  std::shared_ptr<core::Window> get_window_by_title(std::string title) const override;
  [[nodiscard]]
  std::vector<std::string> get_all_titles() const override;
};

}  // namespace windows
}  // namespace appwindows
