#include "window.h"

#include "../core/exceptions/window_does_not_valid.h"
#include "../core/geometry/point.h"
#include "../core/geometry/size.h"

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <ApplicationServices/ApplicationServices.h>
#import <Accessibility/Accessibility.h>

#ifndef kAXWindowIdAttribute
static const CFStringRef kAXWindowIdAttribute = CFSTR("AXWindowId");
#endif

#ifndef kCFNumberCGWindowIDType
#define kCFNumberCGWindowIDType kCFNumberSInt32Type
#endif

static const CFStringRef kAXWindowNumberAttribute = CFSTR("AXWindowNumber");
static const CFStringRef kAXFullScreenAttribute   = CFSTR("AXFullScreen");
static const CFStringRef kAXCloseAction           = CFSTR("AXClose");

namespace appwindows::macos {

void WindowMacOS::set_minimize(bool is_minimize) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    auto pid = *get_process_id();
    @autoreleasepool {
        AXUIElementRef window_element = get_window_element();
        if (!window_element) throw core::exceptions::WindowDoesNotValidException();
        CFBooleanRef value = is_minimize ? kCFBooleanTrue : kCFBooleanFalse;
        AXUIElementSetAttributeValue(window_element, kAXMinimizedAttribute, value);
        CFRelease(window_element);
    }
}

void WindowMacOS::set_fullscreen(bool is_fullscreen) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    auto pid = *get_process_id();
    @autoreleasepool {
        AXUIElementRef window_element = get_window_element();
        if (!window_element) throw core::exceptions::WindowDoesNotValidException();
        CFBooleanRef value = is_fullscreen ? kCFBooleanTrue : kCFBooleanFalse;
        AXUIElementSetAttributeValue(window_element, kAXFullScreenAttribute, value);
        CFRelease(window_element);
    }
}

void WindowMacOS::resize(core::Size size) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    auto pid = *get_process_id();
    @autoreleasepool {
        AXUIElementRef window_element = get_window_element();
        if (!window_element) throw core::exceptions::WindowDoesNotValidException();

        CGSize new_size = CGSizeMake(static_cast<CGFloat>(size.get_width()),
                                     static_cast<CGFloat>(size.get_height()));
        AXValueRef size_value = AXValueCreate(kAXValueTypeCGSize, &new_size);
        if (!size_value) {
            CFRelease(window_element);
            throw core::exceptions::WindowDoesNotValidException();
        }

        AXError result = AXUIElementSetAttributeValue(window_element, kAXSizeAttribute, size_value);
        CFRelease(size_value);
        CFRelease(window_element);
        if (result != kAXErrorSuccess) throw core::exceptions::WindowDoesNotValidException();
    }
}

void WindowMacOS::move(core::Point point) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    auto pid = *get_process_id();
    @autoreleasepool {
        AXUIElementRef window_element = get_window_element();
        if (!window_element) throw core::exceptions::WindowDoesNotValidException();

        CGPoint new_position = CGPointMake(static_cast<CGFloat>(point.get_x()),
                                           static_cast<CGFloat>(point.get_y()));
        AXValueRef position_value = AXValueCreate(kAXValueTypeCGPoint, &new_position);
        if (!position_value) {
            CFRelease(window_element);
            throw core::exceptions::WindowDoesNotValidException();
        }
        AXError result = AXUIElementSetAttributeValue(window_element, kAXPositionAttribute, position_value);
        CFRelease(position_value);
        CFRelease(window_element);
        if (result != kAXErrorSuccess) throw core::exceptions::WindowDoesNotValidException();
    }
}

void WindowMacOS::close() {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    auto pid = *get_process_id();
    @autoreleasepool {
        AXUIElementRef window_element = get_window_element();
        if (!window_element) throw core::exceptions::WindowDoesNotValidException();
        AXUIElementPerformAction(window_element, kAXCloseAction);
        CFRelease(window_element);
    }
}

void WindowMacOS::to_foreground() {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    auto pid = *get_process_id();
    @autoreleasepool {
        AXUIElementRef app_element = AXUIElementCreateApplication(pid);
        if (!app_element) throw core::exceptions::WindowDoesNotValidException();
        AXUIElementSetAttributeValue(app_element, kAXFrontmostAttribute, kCFBooleanTrue);
        CFRelease(app_element);
    }
}

void WindowMacOS::to_background() {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    auto pid = *get_process_id();
    @autoreleasepool {
        AXUIElementRef app_element = AXUIElementCreateApplication(pid);
        if (!app_element) throw core::exceptions::WindowDoesNotValidException();
        AXUIElementSetAttributeValue(app_element, kAXFrontmostAttribute, kCFBooleanFalse);
        CFRelease(app_element);
    }
}

}  // namespace appwindows::macos