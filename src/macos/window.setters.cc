#include "window.h"

#include <ApplicationServices/ApplicationServices.h>
#include <Accessibility/Accessibility.h>
#include <CoreFoundation/CoreFoundation.h>

#include <cstdlib>
#include <memory>
#include <string>

#include "../core/exceptions/window_does_not_valid.h"
#include "../core/geometry/point.h"
#include "../core/geometry/size.h"

namespace appwindows::macos {

AXUIElementRef get_window_element(pid_t pid, CGWindowID window_id) {
    AXUIElementRef app = AXUIElementCreateApplication(pid);
    if (!app) return nullptr;

    CFArrayRef windows;
    AXError result = AXUIElementCopyAttributeValue(app, kAXWindowsAttribute, (CFTypeRef*)&windows);

    if (result != kAXErrorSuccess) {
        CFRelease(app);
        return nullptr;
    }

    CFIndex count = CFArrayGetCount(windows);
    AXUIElementRef target_window = nullptr;

    for (CFIndex i = 0; i < count; ++i) {
        AXUIElementRef window = (AXUIElementRef)CFArrayGetValueAtIndex(windows, i);
        CFNumberRef window_number;
        if (AXUIElementCopyAttributeValue(window, kAXWindowNumberAttribute, (CFTypeRef*)&window_number) == kAXErrorSuccess) {
            int win_id;
            CFNumberGetValue(window_number, kCFNumberIntType, &win_id);
            CFRelease(window_number);

            if (win_id == window_id) {
                target_window = (AXUIElementRef)CFRetain(window);
                break;
            }
        }
    }

    CFRelease(windows);
    CFRelease(app);
    return target_window;
}

void WindowMacOS::set_minimize(bool is_minimize) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    auto pid = *get_process_id();
    AXUIElementRef window_element = get_window_element(pid, window_id_);
    if (!window_element) {
        throw core::exceptions::WindowDoesNotValidException();
    }
    CFBooleanRef value = is_minimize ? kCFBooleanTrue : kCFBooleanFalse;
    AXUIElementSetAttributeValue(window_element, kAXMinimizedAttribute, value);
    CFRelease(window_element);
}

void WindowMacOS::set_fullscreen(bool is_fullscreen) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    auto pid = *get_process_id();
    AXUIElementRef window_element = get_window_element(pid, window_id_);
    if (!window_element) {
        throw core::exceptions::WindowDoesNotValidException();
    }
    CFBooleanRef value = is_fullscreen ? kCFBooleanTrue : kCFBooleanFalse;
    AXUIElementSetAttributeValue(window_element, kAXFullScreenAttribute, value);
    CFRelease(window_element);
}

void WindowMacOS::resize(core::Size size) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    auto pid = *get_process_id();
    AXUIElementRef window_element = get_window_element(pid, window_id_);
    if (!window_element) {
        throw core::exceptions::WindowDoesNotValidException();
    }
    CFTypeRef position_value;
    if (AXUIElementCopyAttributeValue(window_element, kAXPositionAttribute, &position_value) != kAXErrorSuccess) {
        CFRelease(window_element);
        throw core::exceptions::WindowDoesNotValidException();
    }
    CGSize new_size = CGSizeMake(static_cast<CGFloat>(size.get_width()),
    CFTypeRef size_value = AXValueCreate(kAXValueCGSizeType, &new_size);
    if (!size_value) {
        CFRelease(position_value);
        CFRelease(window_element);
        throw core::exceptions::WindowDoesNotValidException();
    }
    AXError result = AXUIElementSetAttributeValue(window_element, kAXSizeAttribute, size_value);
    CFRelease(size_value);
    CFRelease(position_value);
    CFRelease(window_element);
    if (result != kAXErrorSuccess) {
        throw core::exceptions::WindowDoesNotValidException();
    }
}

void WindowMacOS::move(core::Point point) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    auto pid = *get_process_id();
    AXUIElementRef window_element = get_window_element(pid, window_id_);
    if (!window_element) {
        throw core::exceptions::WindowDoesNotValidException();
    }
    CFTypeRef size_value;
    if (AXUIElementCopyAttributeValue(window_element, kAXSizeAttribute, &size_value) != kAXErrorSuccess) {
        CFRelease(window_element);
        throw core::exceptions::WindowDoesNotValidException();
    }
    CGPoint new_position = CGPointMake(static_cast<CGFloat>(point.get_x()),
    CFTypeRef position_value = AXValueCreate(kAXValueCGPointType, &new_position);
    if (!position_value) {
        CFRelease(size_value);
        CFRelease(window_element);
        throw core::exceptions::WindowDoesNotValidException();
    }
    AXError result = AXUIElementSetAttributeValue(window_element, kAXPositionAttribute, position_value);
    CFRelease(position_value);
    CFRelease(size_value);
    CFRelease(window_element);
    if (result != kAXErrorSuccess) {
        throw core::exceptions::WindowDoesNotValidException();
    }
}

void WindowMacOS::close() {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    auto pid = *get_process_id();
    AXUIElementRef window_element = get_window_element(pid, window_id_);
    if (!window_element) {
        throw core::exceptions::WindowDoesNotValidException();
    }
    AXUIElementPerformAction(window_element, kAXCloseAction);
    CFRelease(window_element);
}

void WindowMacOS::to_foreground() {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    auto pid = *get_process_id();
    AXUIElementRef app_element = AXUIElementCreateApplication(pid);
    if (!app_element) {
        throw core::exceptions::WindowDoesNotValidException();
    }
    AXUIElementSetAttributeValue(app_element, kAXFrontmostAttribute, kCFBooleanTrue);
    CFRelease(app_element);
}

void WindowMacOS::to_background() {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    auto pid = *get_process_id();
    AXUIElementRef app_element = AXUIElementCreateApplication(pid);
    if (!app_element) {
        throw core::exceptions::WindowDoesNotValidException();
    }
    AXUIElementSetAttributeValue(app_element, kAXFrontmostAttribute, kCFBooleanFalse);
    CFRelease(app_element);
}

}  // namespace appwindows::macos