#pragma once

#include <string>

namespace appwindows::core {

class Size {
 public:
  bool operator==(const Size& other) const;
  bool operator!=(const Size& other) const;

  [[nodiscard]] virtual int get_width() const noexcept = 0;
  [[nodiscard]] virtual int get_height() const noexcept = 0;
  [[nodiscard]] virtual int to_string() const noexcept = 0;
  [[nodiscard]] virtual std::string to_string() const noexcept = 0;
};

}  // namespace appwindows::core