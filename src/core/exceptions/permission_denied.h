#pragma once

#include <stdexcept>

namespace appwindows::core::exceptions {

class PermissionDeniedException final : public std::runtime_error {
 public:
  explicit PermissionDeniedException()
      : runtime_error("You don't have enough rights for this action.") {}
};

}  // namespace appwindows::core::exceptions