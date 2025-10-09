#pragma once

#include <stdexcept>

namespace appwindows::core::exceptions {

class WindowDoesNotExistException final : public std::runtime_error {
 public:
  explicit WindowDoesNotExistException()
      : runtime_error("Window does not exist") {}
};

}  // namespace appwindows::core::exceptions