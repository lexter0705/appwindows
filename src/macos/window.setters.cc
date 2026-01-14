#include "window.h"

#include <ApplicationServices/ApplicationServices.h>
#include <ApplicationServices/AXValue.h>


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
  AXUIElementRef app = AXUIElementCreateApplication(pid);
  CFArrayRef windows;
  AXUIElementCopyAttributeValues(app, kAXWindowsAttribute, 0, 100, &windows);
  if (windows && CFArrayGetCount(windows) > 0) {
    AXUIElementRef window = (AXUIElementRef)CFArrayGetValueAtIndex(windows, 0);
    CFTypeRef positionRef;
    AXUIElementCopyAttributeValue(window, kAXPositionAttribute, &positionRef);
    CGPoint newPoint = {0, 0};
    if (positionRef) {
      AXValueGetValue((AXValueRef)positionRef, kAXValueCGPointType, &newPoint);
      CFRelease(positionRef);
    }
    CGSize newSize = {static_cast<CGFloat>(size.get_width()),
                      static_cast<CGFloat>(size.get_height())};
    AXValueRef sizeValue = AXValueCreate(kAXValueCGSizeType, &newSize);
    AXUIElementSetAttributeValue(window, kAXSizeAttribute, sizeValue);
    CFRelease(sizeValue);
  }
  if (windows) CFRelease(windows);
  CFRelease(app);
}

void WindowMacOS::move(core::Point point) {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
  auto pid = *get_process_id();
  AXUIElementRef app = AXUIElementCreateApplication(pid);
  CFArrayRef windows;
  AXUIElementCopyAttributeValues(app, kAXWindowsAttribute, 0, 100, &windows);
  if (windows && CFArrayGetCount(windows) > 0) {
    AXUIElementRef window = (AXUIElementRef)CFArrayGetValueAtIndex(windows, 0);
    CGPoint newPoint = {static_cast<CGFloat>(point.get_x()),
                        static_cast<CGFloat>(point.get_y())};
    AXValueRef pointValue = AXValueCreate(kAXValueCGPointType, &newPoint);
    AXUIElementSetAttributeValue(window, kAXPositionAttribute, pointValue);
    CFRelease(pointValue);
  }
  if (windows) CFRelease(windows);
  CFRelease(app);
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