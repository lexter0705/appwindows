#include "finder.h"

#include <memory>
#include <vector>

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <Accessibility/Accessibility.h>

#include "../core/core.h"

#include "window.h"

namespace appwindows::macos {

FinderMacOS::FinderMacOS() = default;

std::vector<std::shared_ptr<core::Window>> FinderMacOS::get_all_windows() const {
  std::vector<std::shared_ptr<core::Window>> windows;
  NSArray* running_apps = [[NSWorkspace sharedWorkspace] runningApplications];
  for (NSRunningApplication* app in running_apps) {
    pid_t pid = [app processIdentifier];
    AXUIElementRef app_ref = AXUIElementCreateApplication(pid);
    if (!app_ref) continue;
    CFTypeRef window_array_ref = nullptr;
    AXError error = AXUIElementCopyAttributeValue(app_ref, CFSTR("AXWindows"), 
                                                  &window_array_ref);
    if (error != kAXErrorSuccess || !window_array_ref) {
      CFRelease(app_ref);
      continue;
    }
    CFArrayRef window_array = reinterpret_cast<CFArrayRef>(window_array_ref);
    CFIndex window_count = CFArrayGetCount(window_array);
    for (CFIndex i = 0; i < window_count; ++i) {
      AXUIElementRef window_ref = reinterpret_cast<AXUIElementRef>(
          const_cast<void*>(CFArrayGetValueAtIndex(window_array, i)));
      CFRetain(window_ref);
      auto window = std::make_shared<WindowMacOS>(window_ref);
      windows.push_back(window);
    }
    CFRelease(window_array_ref);
    CFRelease(app_ref);
  }
  return windows;
}

std::unique_ptr<std::string> FinderMacOS::get_os() const {
  return std::make_unique<std::string>("macos::aqua");
}

}  // namespace appwindows::macos