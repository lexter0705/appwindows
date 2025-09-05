#pragma once

#include <memory>
#include <string>

#include "../core/finder.h"
#include "../core/window.h"

namespace appwindows {
namespace macos {

class FinderMacos final : public core::Finder {
 public:
  FinderMacos();
  [[nodiscard]] std::shared_ptr<core::Window> get_window_by_title(
      std::string title) const override;
  [[nodiscard]] std::vector<std::shared_ptr<core::Window>> get_all_windows()
      const override;
};

}  // namespace macos
}  // namespace appwindows
