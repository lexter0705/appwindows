#include "window.h"

#include "../core/exceptions/window_does_not_valid.h"

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <ApplicationServices/ApplicationServices.h>
#import <Accessibility/Accessibility.h>

#ifndef kAXWindowIdAttribute
static const CFStringRef kAXWindowIdAttribute = CFSTR("AXWindowId");
#endif

#ifndef kAXMinSizeAttribute
static const CFStringRef kAXMinSizeAttribute = CFSTR("AXMinSize");
#endif

#ifndef kAXMaxSizeAttribute
static const CFStringRef kAXMaxSizeAttribute = CFSTR("AXMaxSize");
#endif

#ifndef kCFNumberCGWindowIDType
#define kCFNumberCGWindowIDType kCFNumberSInt32Type
#endif

namespace appwindows::macos {

AXUIElementRef get_window_element(pid_t pid, CGWindowID window_id) {
  @autoreleasepool {
    AXUIElementRef app = AXUIElementCreateApplication(pid);
    if (!app) return nullptr;
    CFArrayRef windows = nullptr;
    AXError err = AXUIElementCopyAttributeValue(app, kAXWindowsAttribute, (CFTypeRef*)&windows);
    if (err != kAXErrorSuccess || !windows) {
      CFRelease(app);
      return nullptr;
    }
    AXUIElementRef result = nullptr;
    CFIndex count = CFArrayGetCount(windows);
    for (CFIndex i = 0; i < count; ++i) {
      AXUIElementRef window = (AXUIElementRef)CFArrayGetValueAtIndex(windows, i);
      if (!window) continue;
      CFTypeRef value = nullptr;
      err = AXUIElementCopyAttributeValue(window, kAXWindowIdAttribute, &value);
      if (err == kAXErrorSuccess && value && CFGetTypeID(value) == CFNumberGetTypeID()) {
        CGWindowID ax_win_id = 0;
        if (CFNumberGetValue((CFNumberRef)value, kCFNumberCGWindowIDType, &ax_win_id)) {
          if (ax_win_id == window_id) {
            result = (AXUIElementRef)CFRetain(window);
            CFRelease(value);
            break;
          }
        }
      }
      if (value) CFRelease(value);
    }
    CFRelease(windows);
    CFRelease(app);
    return result;
  }
}

std::unique_ptr<core::Size> WindowMacOS::get_min_size() const {
  if (!*is_valid()) {
    throw core::exceptions::WindowDoesNotValidException();
  }
  auto pid = *get_process_id();
  @autoreleasepool {
    AXUIElementRef window_element = get_window_element(pid, window_id_);
    if (!window_element) {
      throw core::exceptions::WindowDoesNotValidException();
    }
    CFTypeRef value = nullptr;
    AXError err = AXUIElementCopyAttributeValue(window_element, kAXMinSizeAttribute, &value);
    CFRelease(window_element);
    if (err != kAXErrorSuccess || !value) {
      return nullptr;
    }
    if (CFGetTypeID(value) != AXValueGetTypeID()) {
      CFRelease(value);
      return nullptr;
    }
    CGSize size;
    if (!AXValueGetValue((AXValueRef)value, kAXValueTypeCGSize, &size)) {
      CFRelease(value);
      return nullptr;
    }
    CFRelease(value);
    return std::make_unique<core::Size>(
        static_cast<int>(size.width),
        static_cast<int>(size.height));
  }
}

std::unique_ptr<core::Size> WindowMacOS::get_max_size() const {
  if (!*is_valid()) {
    throw core::exceptions::WindowDoesNotValidException();
  }
  auto pid = *get_process_id();
  @autoreleasepool {
    AXUIElementRef window_element = get_window_element(pid, window_id_);
    if (!window_element) {
      throw core::exceptions::WindowDoesNotValidException();
    }
    CFTypeRef value = nullptr;
    AXError err = AXUIElementCopyAttributeValue(window_element, kAXMaxSizeAttribute, &value);
    CFRelease(window_element);
    if (err != kAXErrorSuccess || !value) {
      return nullptr;
    }
    if (CFGetTypeID(value) != AXValueGetTypeID()) {
      CFRelease(value);
      return nullptr;
    }
    CGSize size;
    if (!AXValueGetValue((AXValueRef)value, kAXValueTypeCGSize, &size)) {
      CFRelease(value);
      return nullptr;
    }
    CFRelease(value);
    return std::make_unique<core::Size>(
        static_cast<int>(size.width),
        static_cast<int>(size.height));
  }
}

} // namespace appwindows::macos
