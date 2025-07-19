#include "platform.h"

#include <pybind11/pybind11.h>

#include "../windows/finder.h"
#include "../x_server/finder.h"

namespace appwindows {
std::shared_ptr<core::Finder> get_finder() {
#ifdef _WIN32 || _WIN64
  return std::make_shared<windows::FinderWindows>();
#elif __linux__
  return std::make_shared<x_server::FinderXServer>();
#else
#error "Unsoported platform"
#endif
}
}  // namespace appwindows