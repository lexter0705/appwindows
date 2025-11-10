#include "finder.h"

#include <memory>

#include "../core/window.h"
#include "window.h"

namespace appwindows {
namespace macos {

FinderMacos::FinderMacos() = default;

std::shared_ptr<core::Window> FinderMacos::get_window_by_title(
    const std::string title) const {
  return nullptr;
}

std::vector<std::shared_ptr<core::Window>> FinderMacos::get_all_windows()
    const {
  return nullptr;
}

}  // namespace macos
}  // namespace appwindows