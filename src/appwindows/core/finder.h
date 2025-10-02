#pragma once

#include <memory>
#include <vector>
#include <string>

#include "window.h"

namespace appwindows {
namespace core {

class Finder {
 public:
  virtual ~Finder() = default;
  [[nodiscard]]
  virtual std::shared_ptr<Window> get_window_by_title(
      std::string title) const = 0;
  [[nodiscard]]
  virtual std::vector<std::shared_ptr<Window>> get_all_windows() const = 0;
  [[nodiscard]]
  virtual std::shared_ptr<Window> get_window_by_process_id(int process_id) const = 0;
  [[nodiscard]]
  std::vector<std::string> get_all_titles() const;
};

}  // namespace core
}  // namespace appwindows
