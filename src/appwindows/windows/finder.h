#pragma once
#ifdef _WIN32 || _WIN64

#include <memory>
#include <string>

#include "../core/finder.h"
#include "../core/window.h"

namespace appwindows {
namespace windows {

class FinderWindows final : public core::FinderI {
 public:
  FinderWindows() = default;
  [[nodiscard]] std::unique_ptr<core::WindowI> get_window_by_title(
      std::string title) const override;
};

}  // namespace windows
}  // namespace appwindows

#endif