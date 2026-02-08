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

#ifndef kAXValueTypeCGSize
#define kAXValueTypeCGSize 2
#endif

#ifndef kAXValueTypeCGPoint
#define kAXValueTypeCGPoint 1
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
    CGFloat width = static_cast<CGFloat>(size.get_width());
    CGFloat height = static_cast<CGFloat>(size.get_height());
    [UIElementUtilities setStringValue:[NSString stringWithFormat:@"w=%f h=%f", width, height]
                          forAttribute:@"AXSize" ofUIElement:window_ref_]
}

void WindowMacOS::move(core::Point point) {
    if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
    CGFloat x = static_cast<CGFloat>(point.get_x());
    CGFloat y = static_cast<CGFloat>(point.get_y());
    [UIElementUtilities setStringValue:[NSString stringWithFormat:@"x=%f y=%f", x, y]
                          forAttribute:@"AXPosition" ofUIElement:window_ref_]
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