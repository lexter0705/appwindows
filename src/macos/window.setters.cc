#include "window.h"

#include <ApplicationServices/ApplicationServices.h>

#include <cstdlib>
#include <memory>
#include <string>

#include "../core/exceptions/window_does_not_valid.h"
#include "../core/geometry/point.h"
#include "../core/geometry/size.h"

namespace appwindows::macos {

void execute_apple_script(const std::string& script) {
  std::string command = "osascript -e '" + script + "'";
  system(command.c_str());
}

void WindowMacOS::set_minimize(bool is_minimize) {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();

  auto pid = *get_process_id();
  std::string script;
  if (is_minimize) {
    script =
        "tell application \"System Events\"\n"
        "  set frontmost of process (unix id " +
        std::to_string(pid) +
        ") to true\n"
        "  tell application process (unix id " +
        std::to_string(pid) +
        ")\n"
        "    set value of attribute \"AXMinimized\" to true\n"
        "  end tell\n"
        "end tell";
  } else {
    script =
        "tell application \"System Events\"\n"
        "  set frontmost of process (unix id " +
        std::to_string(pid) +
        ") to true\n"
        "  tell application process (unix id " +
        std::to_string(pid) +
        ")\n"
        "    set value of attribute \"AXMinimized\" to false\n"
        "  end tell\n"
        "end tell";
  }

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
  CFArrayRef window_list = CGWindowListCopyWindowInfo(
      kCGWindowListOptionIncludingWindow, window_id_);

  if (CFArrayGetCount(window_list) == 0) {
    CFRelease(window_list);
    throw core::exceptions::WindowDoesNotValidException();
  }

  CFDictionaryRef window_info =
      reinterpret_cast<CFDictionaryRef>(CFArrayGetValueAtIndex(window_list, 0));
  CFDictionaryRef bounds_ref = reinterpret_cast<CFDictionaryRef>(
      CFDictionaryGetValue(window_info, kCGWindowBounds));

  CGRect bounds;
  CGRectMakeWithDictionaryRepresentation(bounds_ref, &bounds);
  CFRelease(window_list);

  CGRect new_bounds = CGRectMake(bounds.origin.x, bounds.origin.y,
                                 static_cast<CGFloat>(size.get_width()),
                                 static_cast<CGFloat>(size.get_height()));

  std::string script =
      "tell application \"System Events\"\n"
      "  set frontmost of process (unix id " +
      std::to_string(pid) +
      ") to true\n"
      "  tell application process (unix id " +
      std::to_string(pid) +
      ")\n"
      "    tell front window\n"
      "      set bounds to {" +
      std::to_string(static_cast<int>(new_bounds.origin.x)) + ", " +
      std::to_string(static_cast<int>(new_bounds.origin.y)) + ", " +
      std::to_string(
          static_cast<int>(new_bounds.origin.x + new_bounds.size.width)) +
      ", " +
      std::to_string(
          static_cast<int>(new_bounds.origin.y + new_bounds.size.height)) +
      "}\n"
      "    end tell\n"
      "  end tell\n"
      "end tell";

  execute_apple_script(script);
}

void WindowMacOS::move(core::Point point) {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();

  auto pid = *get_process_id();
  CFArrayRef window_list = CGWindowListCopyWindowInfo(
      kCGWindowListOptionIncludingWindow, window_id_);

  if (CFArrayGetCount(window_list) == 0) {
    CFRelease(window_list);
    throw core::exceptions::WindowDoesNotValidException();
  }

  CFDictionaryRef window_info =
      reinterpret_cast<CFDictionaryRef>(CFArrayGetValueAtIndex(window_list, 0));
  CFDictionaryRef bounds_ref = reinterpret_cast<CFDictionaryRef>(
      CFDictionaryGetValue(window_info, kCGWindowBounds));

  CGRect bounds;
  CGRectMakeWithDictionaryRepresentation(bounds_ref, &bounds);
  CFRelease(window_list);

  CGRect new_bounds = CGRectMake(static_cast<CGFloat>(point.get_x()),
                                 static_cast<CGFloat>(point.get_y()),
                                 bounds.size.width, bounds.size.height);

  std::string script =
      "tell application \"System Events\"\n"
      "  set frontmost of process (unix id " +
      std::to_string(pid) +
      ") to true\n"
      "  tell application process (unix id " +
      std::to_string(pid) +
      ")\n"
      "    tell front window\n"
      "      set bounds to {" +
      std::to_string(static_cast<int>(new_bounds.origin.x)) + ", " +
      std::to_string(static_cast<int>(new_bounds.origin.y)) + ", " +
      std::to_string(
          static_cast<int>(new_bounds.origin.x + new_bounds.size.width)) +
      ", " +
      std::to_string(
          static_cast<int>(new_bounds.origin.y + new_bounds.size.height)) +
      "}\n"
      "    end tell\n"
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