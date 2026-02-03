#include "finder.h"

#include <memory>
#include <vector>

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <Accessibility/Accessibility.h>
#import <CoreGraphics/CoreGraphics.h>

#include "../core/base_window.h"
#include "window.h"

namespace appwindows::macos {

#ifnded kAXWindowsAttribute
static CFStringRef kAXWindowsAttribute = CFSTR("AXWindows");
#endif
  
#ifnded kAXWindowsAttribute
static CFStringRef kAXMainWindowAttribute = CFSTR("AXMainWindow");
#endif

#ifnded kAXWindowsAttribute
static CFStringRef kAXFocusedWindowAttribute = CFSTR("AXFocusedWindow");
#endif
  
FinderMacOS::FinderMacOS() = default;

std::vector<std::shared_ptr<core::Window>> FinderMacOS::get_all_windows() const {
  std::vector<std::shared_ptr<core::Window>> windows;
  CGWindowListOption list_options = kCGWindowListOptionOnScreenOnly |
                                   kCGWindowListExcludeDesktopElements;
  CFArrayRef window_list = CGWindowListCopyWindowInfo(list_options, kCGNullWindowID);
  if (!window_list) {
    return windows;
  }
  CFIndex count = CFArrayGetCount(window_list);
  for (CFIndex i = 0; i < count; ++i) {
    CFDictionaryRef window_info = reinterpret_cast<CFDictionaryRef>(
        CFArrayGetValueAtIndex(window_list, i));
    CFNumberRef window_id_ref = reinterpret_cast<CFNumberRef>(
        CFDictionaryGetValue(window_info, kCGWindowNumber));
    if (!window_id_ref) continue;
    CGWindowID window_id = 0;
    CFNumberGetValue(window_id_ref, kCFNumberIntType, &window_id);
    CFNumberRef owner_pid_ref = reinterpret_cast<CFNumberRef>(
        CFDictionaryGetValue(window_info, kCGWindowOwnerPID));
    if (!owner_pid_ref) continue;
    pid_t owner_pid = 0;
    CFNumberGetValue(owner_pid_ref, kCFNumberIntType, &owner_pid);
    AXUIElementRef app_ref = AXUIElementCreateApplication(owner_pid);
    if (!app_ref) continue;
    CFTypeRef window_array_ref = nullptr;
    AXError error = AXUIElementCopyAttributeValue(app_ref, kAXWindowsAttribute,
                                                  &window_array_ref);
    if (error != kAXErrorSuccess || !window_array_ref) {
      CFRelease(app_ref);
      continue;
    }
    CFArrayRef window_array = reinterpret_cast<CFArrayRef>(window_array_ref);
    CFIndex window_count = CFArrayGetCount(window_array);
    for (CFIndex j = 0; j < window_count; ++j) {
      AXUIElementRef window_ref = reinterpret_cast<AXUIElementRef>(
          const_cast<void*>(CFArrayGetValueAtIndex(window_array, j)));
      CFRetain(window_ref);
      CFTypeRef window_id_attr_ref = nullptr;
      AXError id_error = AXUIElementCopyAttributeValue(
          window_ref, CFSTR("AXWindowIdentifier"), &window_id_attr_ref);
      if (id_error == kAXErrorSuccess && window_id_attr_ref) {
        CGWindowID ax_window_id = 0;
        if (CFGetTypeID(window_id_attr_ref) == CFNumberGetTypeID())
          CFNumberGetValue(reinterpret_cast<CFNumberRef>(window_id_attr_ref),
                          kCFNumberIntType, &ax_window_id);
        CFRelease(window_id_attr_ref);
        if (ax_window_id == window_id) {
          auto window = std::make_shared<WindowMacOS>(window_ref);
          windows.push_back(window);
          break;
        }
      } else {
        CFTypeRef window_number_ref = nullptr;
        AXError number_error = AXUIElementCopyAttributeValue(
            window_ref, kAXWindowNumberAttribute, &window_number_ref);
        if (number_error == kAXErrorSuccess && window_number_ref) {
          int ax_window_number = 0;
          if (CFGetTypeID(window_number_ref) == CFNumberGetTypeID()) {
            CFNumberGetValue(reinterpret_cast<CFNumberRef>(window_number_ref),
                            kCFNumberIntType, &ax_window_number);
          }
          CFRelease(window_number_ref);

          if (static_cast<CGWindowID>(ax_window_number) == window_id) {
            auto window = std::make_shared<WindowMacOS>(window_ref);
            windows.push_back(window);
            break;
          }
        }
      }
      CFRelease(window_ref);
    }
    CFRelease(window_array_ref);
    CFRelease(app_ref);
  }
  CFRelease(window_list);
  NSArray* running_apps = [[NSWorkspace sharedWorkspace] runningApplications];
  for (NSRunningApplication* app in running_apps) {
    pid_t pid = [app processIdentifier];
    AXUIElementRef app_ref = AXUIElementCreateApplication(pid);
    if (!app_ref) continue;
    CFTypeRef focused_window_ref = nullptr;
    AXError focused_error = AXUIElementCopyAttributeValue(
        app_ref, kAXFocusedWindowAttribute, &focused_window_ref);
    if (focused_error == kAXErrorSuccess && focused_window_ref) {
      CFRetain(focused_window_ref);
      bool already_exists = false;
      for (const auto& window : windows) {
        auto window_macos = std::dynamic_pointer_cast<WindowMacOS>(window);
        if (window_macos) {
          CFTypeRef window_ref = window_macos->window_ref_;
          if (window_ref == focused_window_ref) {
            already_exists = true;
            break;
          }
        }
      }
      if (!already_exists) {
        auto window = std::make_shared<WindowMacOS>(
            reinterpret_cast<AXUIElementRef>(focused_window_ref));
        windows.push_back(window);
      } else
        CFRelease(focused_window_ref);
    }
    if (focused_window_ref)
      CFRelease(focused_window_ref);
    CFRelease(app_ref);
  }

  return windows;
}

std::unique_ptr<std::string> FinderMacOS::get_os() const {
  return std::make_unique<std::string>("macos::aqua");
}

}  // namespace appwindows::macos
