#pragma once

#include <memory>

#include "window.h"

namespace appwindows {
namespace core {

class FinderI {
 public:
  virtual ~FinderI() = default;
  [[nodiscard]]
  virtual std::unique_ptr<WindowI> get_window_by_title(
      std::string title) const = 0;
};

}  // namespace core
}  // namespace appwindows
