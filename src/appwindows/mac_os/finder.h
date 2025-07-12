#pragma once

#include <memory>
#include <string>

#include "../core/finder.h"
#include "window.h"

namespace appwindows {
namespace windows {

class FinderMacOs final : public core::FinderI {
 public:
  FinderMacOs() = default;
  [[nodiscard]] std::unique_ptr<core::WindowI> get_window_by_title(
      std::string title) const override;
};

}  // namespace windows
}  // namespace appwindows