#include "window.h"

#include <ApplicationServices/ApplicationServices.h>

#include <cstdlib>
#include <memory>
#include <string>
#include <chrono>
#include <thread>

#include "../core/exceptions/window_does_not_valid.h"
#include "../core/geometry/point.h"
#include "../core/geometry/size.h"

namespace appwindows::macos {

void execute_apple_script(const std::string& script) {
  std::string command = "osascript -e '" + script + "'";
  system(command.c_str());
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void WindowMacOS::set_minimize(bool is_minimize) {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
  auto pid = *get_process_id();
  std::string script =
      "tell application \"System Events\"\n"
      "  set frontmost of process (unix id " +
      std::to_string(pid) +
      ") to true\n"
      "  tell application process (unix id " +
      std::to_string(pid) +
      ")\n"
      "    set value of attribute \"AXMinimized\" to" +
      (is_minimize ? "true" : "false") +
      "\n"
      "  end tell\n"
      "end tell";
  execute_apple_script(script);
}

void WindowMacOS::set_fullscreen(bool is_fullscreen) {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
  auto pid = *get_process_id();
  std::string script =
      "tell application \"System Events\"\n"
      "  set frontmost of process (unix id " +
      std::to_string(pid) +
      ") to true\n"
      "  tell application process (unix id " +
      std::to_string(pid) +
      ")\n"
      "    set value of attribute \"AXFullScreen\" to " +
      (is_fullscreen ? "true" : "false") +
      "\n"
      "  end tell\n"
      "end tell";
  execute_apple_script(script);
}

void WindowMacOS::resize(core::Size size) {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
  auto pid = *get_process_id();
  std::string script =
      "tell application \"System Events\"\n"
      "  set frontmost of process (unix id " +
      std::to_string(pid) +
      ") to true\n"
      "  tell application process (unix id " +
      std::to_string(pid) +
      ")\n"
      "    set the_bounds to bounds of front window\n"
      "    set item 3 of the_bounds to " +
      std::to_string(size.get_width()) +
      "\n"
      "    set item 4 of the_bounds to " +
      std::to_string(size.get_height()) +
      "\n"
      "    set bounds of front window to the_bounds\n"
      "  end tell\n"
      "end tell";
  execute_apple_script(script);
}

void WindowMacOS::move(core::Point point) {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
  auto pid = *get_process_id();
  std::string script =
      "tell application \"System Events\"\n"
      "  set frontmost of process (unix id " +
      std::to_string(pid) +
      ") to true\n"
      "  tell application process (unix id " +
      std::to_string(pid) +
      ")\n"
      "    set the_bounds to bounds of front window\n"
      "    set item 1 of the_bounds to " +
      std::to_string(point.get_x()) +
      "\n"
      "    set item 2 of the_bounds to " +
      std::to_string(point.get_y()) +
      "\n"
      "    set bounds of front window to the_bounds\n"
      "  end tell\n"
      "end tell";
  execute_apple_script(script);
}

void WindowMacOS::close() {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();

    auto pid = *get_process_id();
    std::string script =
        "tell application \"System Events\"\n"
        "  set frontmost of process (unix id " +
        std::to_string(pid) +
        ") to true\n"
        "  tell application process (unix id " +
        std::to_string(pid) +
        ")\n"
        "    tell front window\n"
        "      perform action \"AXClose\"\n"
        "    end tell\n"
        "  end tell\n"
        "end tell";

    execute_apple_script(script);
}

void WindowMacOS::to_foreground() {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();

    auto pid = *get_process_id();
    std::string script =
        "tell application \"System Events\"\n"
        "  set frontmost of process (unix id " +
        std::to_string(pid) +
        ") to true\n"
        "  tell application process (unix id " +
        std::to_string(pid) +
        ")\n"
        "    set frontmost to true\n"
        "  end tell\n"
        "end tell";

    execute_apple_script(script);
}

void WindowMacOS::to_background() {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();

    auto pid = *get_process_id();
    std::string script =
        "tell application \"System Events\"\n"
        "  tell application process (unix id " +
        std::to_string(pid) +
        ")\n"
        "    set frontmost to false\n"
        "  end tell\n"
        "end tell";

    execute_apple_script(script);
}

}  // namespace appwindows::macos