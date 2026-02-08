#include "window.h"

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <Accessibility/Accessibility.h>

#include "../core/core.h"

#ifndef kAXWindowIdAttribute
static const CFStringRef kAXWindowIdAttribute = CFSTR("AXWindowId");
#endif

#ifndef kCFNumberCGWindowIDType
#define kCFNumberCGWindowIDType kCFNumberSInt32Type
#endif

namespace appwindows::macos {

void WindowMacOS::set_minimize(bool is_minimize) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    CFBooleanRef minimize_value = is_minimize ? kCFBooleanTrue : kCFBooleanFalse;
    AXError error = AXUIElementSetAttributeValue(
        window_ref_, CFSTR("AXMinimized"), minimize_value);
    handle_error(error);
}

void WindowMacOS::set_fullscreen(bool is_fullscreen) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    CFBooleanRef fullscreen_value = is_fullscreen ? kCFBooleanTrue : kCFBooleanFalse;
    AXError error = AXUIElementSetAttributeValue(
        window_ref_, CFSTR("AXFullScreen"), fullscreen_value);
    handle_error(error);
}

void WindowMacOS::resize(core::Size size) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    CGSize cg_size = {
        static_cast<CGFloat>(size.get_width()),
        static_cast<CGFloat>(size.get_height())
    };
    CFTypeRef current_value = nullptr;
    AXError error = AXUIElementCopyAttributeValue(
        window_ref_, CFSTR("AXSize"), &current_value);
    if (error == kAXErrorSuccess && current_value) {
        AXValueRef size_value = AXValueCreate(kAXValueCGSizeType, &cg_size);
        if (size_value) {
            error = AXUIElementSetAttributeValue(
                window_ref_, CFSTR("AXSize"), size_value);
            CFRelease(size_value);
        }
        CFRelease(current_value);
    }
    handle_error(error);
}

void WindowMacOS::move(core::Point point) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    CGPoint cg_point = {
        static_cast<CGFloat>(point.get_x()),
        static_cast<CGFloat>(point.get_y())
    };
    CFTypeRef current_value = nullptr;
    AXError error = AXUIElementCopyAttributeValue(
        window_ref_, CFSTR("AXPosition"), &current_value);
    if (error == kAXErrorSuccess && current_value) {
        AXValueRef point_value = AXValueCreate(kAXValueCGPointType, &cg_point);
        if (point_value) {
            error = AXUIElementSetAttributeValue(
                window_ref_, CFSTR("AXPosition"), point_value);
            CFRelease(point_value);
        }
        CFRelease(current_value);
    }
    handle_error(error);
}

void WindowMacOS::close() {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    AXError error = AXUIElementPerformAction(window_ref_, CFSTR("AXClose"));
    handle_error(error);
}

void WindowMacOS::to_foreground() {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    pid_t pid;
    AXError error = AXUIElementGetPid(window_ref_, &pid);
    handle_error(error);
    NSRunningApplication* app = [NSRunningApplication runningApplicationWithProcessIdentifier:pid];
    if (!app) throw core::exceptions::WindowDoesNotValidException();
    [app activateWithOptions:NSApplicationActivateIgnoringOtherApps];
    error = AXUIElementPerformAction(window_ref_, CFSTR("AXRaise"));
    handle_error(error);
}

void WindowMacOS::to_background() {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    NSArray* apps = [NSRunningApplication runningApplicationsWithBundleIdentifier:@"com.apple.finder"];
    if ([apps count] == 0) throw core::exceptions::WindowDoesNotValidException();
    NSRunningApplication* finder = [apps firstObject];
    [finder activateWithOptions:NSApplicationActivateIgnoringOtherApps];
}

}  // namespace appwindows::macos