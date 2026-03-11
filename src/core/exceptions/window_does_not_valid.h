#pragma once

#include <stdexcept>

namespace appwindows::core::exceptions {

class WindowDoesNotValidException final : public std::runtime_error {
 public:
  explicit WindowDoesNotValidException()
      : runtime_error("Window does not valid") {}

  explicit WindowDoesNotValidException(const std::string& message)
    : runtime_error("Window does not valid. Message:" + message) {}
};

}  // namespace appwindows::core::exceptions