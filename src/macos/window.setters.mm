#include "window.h"

#include "../core/exceptions/window_does_not_valid.h"
#include "../core/geometry/point.h"
#include "../core/geometry/size.h"

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
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
static const CFStringRef kAXMinimizedAttribute    = CFSTR("AXMinimized");
static const CFStringRef kAXRaiseAction           = CFSTR("AXRaise");
static const CFStringRef kAXPositionAttribute     = CFSTR("AXPosition");
static const CFStringRef kAXSizeAttribute         = CFSTR("AXSize");

namespace appwindows::macos {

void WindowMacOS::set_minimize(bool is_minimize) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    CFBooleanRef minimize_value = is_minimize ? kCFBooleanTrue : kCFBooleanFalse;
    AXError error = AXUIElementSetAttributeValue(
        window_ref_, kAXMinimizedAttribute, minimize_value);
    if (error != kAXErrorSuccess)
        throw core::exceptions::WindowDoesNotValidException();
}

void WindowMacOS::set_fullscreen(bool is_fullscreen) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    CFBooleanRef fullscreen_value = is_fullscreen ? kCFBooleanTrue : kCFBooleanFalse;
    AXError error = AXUIElementSetAttributeValue(
        window_ref_, kAXFullScreenAttribute, fullscreen_value);
    if (error != kAXErrorSuccess)
        throw core::exceptions::WindowDoesNotValidException();
}

void WindowMacOS::resize(core::Size size) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    CGSize cg_size = {static_cast<CGFloat>(size.get_width()),
                      static_cast<CGFloat>(size.get_height())};
    CFTypeRef size_value = AXValueCreate(kAXValueCGSizeType, &cg_size);
    if (!size_value)
        throw core::exceptions::WindowDoesNotValidException();
    AXError error = AXUIElementSetAttributeValue(
        window_ref_, kAXSizeAttribute, size_value);
    CFRelease(size_value);
    if (error != kAXErrorSuccess)
        throw core::exceptions::WindowDoesNotValidException();
}

void WindowMacOS::move(core::Point point) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    CGPoint cg_point = {static_cast<CGFloat>(point.get_x()),
                        static_cast<CGFloat>(point.get_y())};
    CFTypeRef position_value = AXValueCreate(kAXValueCGPointType, &cg_point);
    if (!position_value)
        throw core::exceptions::WindowDoesNotValidException();
    AXError error = AXUIElementSetAttributeValue(
        window_ref_, kAXPositionAttribute, position_value);
    CFRelease(position_value);
    if (error != kAXErrorSuccess)
        throw core::exceptions::WindowDoesNotValidException();
}

void WindowMacOS::close() {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    AXError error = AXUIElementPerformAction(window_ref_, kAXCloseAction);
    if (error != kAXErrorSuccess)
        throw core::exceptions::WindowDoesNotValidException();
}

void WindowMacOS::to_foreground() {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    pid_t pid;
    AXError error = AXUIElementGetPid(window_ref_, &pid);
    if (error != kAXErrorSuccess)
        throw core::exceptions::WindowDoesNotValidException();
    NSRunningApplication* app = [NSRunningApplication runningApplicationWithProcessIdentifier:pid];
    if (!app)
        throw core::exceptions::WindowDoesNotValidException();
    [app activateWithOptions:NSApplicationActivateIgnoringOtherApps];
    error = AXUIElementPerformAction(window_ref_, kAXRaiseAction);
    if (error != kAXErrorSuccess)
        throw core::exceptions::WindowDoesNotValidException();
}

void WindowMacOS::to_background() {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    NSArray* apps = [NSRunningApplication runningApplicationsWithBundleIdentifier:@"com.apple.finder"];
    if ([apps count] == 0)
        throw core::exceptions::WindowDoesNotValidException();
    NSRunningApplication* finder = [apps firstObject];
    [finder activateWithOptions:NSApplicationActivateIgnoringOtherApps];
}

}  // namespace appwindows::macos