#include "window.h"

#include <windows.h>

#include <iostream>

#include "../core/exceptions/window_does_not_exist.h"
#include "../core/geometry/point.h"
#include "../core/geometry/size.h"

namespace appwindows {
namespace macos {

WindowMacos::WindowMacos() {}

void WindowMacos::set_fullscreen(const bool is_fullscreen) {}

void WindowMacos::resize(const core::Size size) {}

void WindowMacos::move(const core::Point point) {}

void WindowMacos::close() {}

void WindowMacos::to_background() {}

void WindowMacos::to_foreground() {}

void WindowMacos::set_minimize(const bool is_minimize) {}

}  // namespace macos
}  // namespace appwindows
