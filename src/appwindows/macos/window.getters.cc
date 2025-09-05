#include "window.h"

#ifndef PW_RENDERFULLCONTENT
#define PW_RENDERFULLCONTENT 0x00000002
#endif

#include "../core/exceptions/window_does_not_exist.h"

namespace appwindows {
namespace macos {

std::unique_ptr<std::string> WindowMacos::get_title() const { return nullptr; }

std::unique_ptr<std::vector<core::Point>> WindowMacos::get_points() {
  return nullptr;
}

std::unique_ptr<core::Size> WindowMacos::get_size() const { return nullptr; }

py::array_t<unsigned char> WindowMacos::get_screenshot() { return nullptr; }

}  // namespace macos
}  // namespace appwindows
