#include "window.h"

#include <memory>

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <Accessibility/Accessibility.h>

#include "../core/core.h"

#ifndef kAXErrorNotEnoughPrivileges
#define kAXErrorNotEnoughPrivileges static_cast<AXError>(-25209)
#endif

#ifndef kAXErrorAPIDisabled
#define kAXErrorAPIDisabled static_cast<AXError>(-25210)
#endif

namespace appwindows::macos {

WindowMacOS::WindowMacOS(AXUIElementRef window_ref) : window_ref_(window_ref) {
  if (window_ref_) CFRetain(window_ref_);
}

WindowMacOS::~WindowMacOS() {
  if (window_ref_) CFRelease(window_ref_);
}

AXUIElementRef WindowMacOS::get_window_ref() const {
  return window_ref_;
}

std::unique_ptr<bool> WindowMacOS::is_valid() const {
  if (!window_ref_)
    return std::make_unique<bool>(false);
  pid_t pid = 0;
  AXError error = AXUIElementGetPid(window_ref_, &pid);
  handle_error(error);
  return std::make_unique<bool>(true);
}

void WindowMacOS::handle_error(AXError error) const {
  if (error == kAXErrorNotEnoughPrivileges || error == kAXErrorAPIDisabled)
    throw core::exceptions::PermissionDeniedException();
  else if (error != kAXErrorSuccess)
    throw core::exceptions::WindowDoesNotValidException("(" + std::to_string(error) + ")");
}

std::unique_ptr<core::QuadPoints> WindowMacOS::get_points() {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
  
  CFTypeRef position_value = nullptr;
  CFTypeRef size_value = nullptr;
  
  AXError position_error = AXUIElementCopyAttributeValue(
      window_ref_, CFSTR("AXPosition"), &position_value);
  AXError size_error = AXUIElementCopyAttributeValue(
      window_ref_, CFSTR("AXSize"), &size_value);
  
  handle_error(size_error);
  handle_error(position_error);
  
  CGPoint position = {0, 0};
  CGSize size = {0, 0};
  if (position_value && CFGetTypeID(position_value) == AXValueGetTypeID()) {
    NSValue* posValue = (__bridge NSValue*)position_value;
    if ([posValue isKindOfClass:[NSValue class]])
      if (strcmp([posValue objCType], @encode(CGPoint)) == 0)
        position = [posValue pointValue];
  }
  if (size_value && CFGetTypeID(size_value) == AXValueGetTypeID()) {
    NSValue* sizeValue = (__bridge NSValue*)size_value;
    if ([sizeValue isKindOfClass:[NSValue class]])
      if (strcmp([sizeValue objCType], @encode(CGSize)) == 0)
        size = [sizeValue sizeValue];
  }
  
  CFRelease(position_value);
  CFRelease(size_value);
  auto left_top = core::Point(static_cast<int>(position.x), 
                              static_cast<int>(position.y));
  auto right_top = core::Point(static_cast<int>(position.x + size.width), 
                               static_cast<int>(position.y));
  auto right_bottom = core::Point(static_cast<int>(position.x + size.width), 
                                  static_cast<int>(position.y + size.height));
  auto left_bottom = core::Point(static_cast<int>(position.x), 
                                 static_cast<int>(position.y + size.height));
  return std::make_unique<core::QuadPoints>(
      left_top, right_top, right_bottom, left_bottom);
}

std::unique_ptr<std::string> WindowMacOS::get_title() const {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
  CFTypeRef title_value = nullptr;
  AXError error = AXUIElementCopyAttributeValue(
      window_ref_, CFSTR("AXTitle"), &title_value);
  if (error != kAXErrorSuccess)
    return std::make_unique<std::string>(std::string());
  CFStringRef title_string = reinterpret_cast<CFStringRef>(title_value);
  CFIndex length = CFStringGetLength(title_string);
  CFIndex max_size = CFStringGetMaximumSizeForEncoding(length,
                                                       kCFStringEncodingUTF8);
  char* buffer = new char[max_size];
  std::unique_ptr<std::string> result;
  if (CFStringGetCString(title_string, buffer, max_size, 
                         kCFStringEncodingUTF8)) 
    result = std::make_unique<std::string>(buffer);
  else 
    result = std::make_unique<std::string>("");

  delete[] buffer;
  CFRelease(title_value);
  return result;
}

std::unique_ptr<core::Size> WindowMacOS::get_size() const {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
  CFTypeRef size_value = nullptr;
  AXError error = AXUIElementCopyAttributeValue(
      window_ref_, CFSTR("AXSize"), &size_value);
  handle_error(error);
  CGSize cg_size = {0, 0};
  if (size_value && CFGetTypeID(size_value) == AXValueGetTypeID()) {
    NSValue* sizeValue = (__bridge NSValue*)size_value;
    if ([sizeValue isKindOfClass:[NSValue class]])
      if (strcmp([sizeValue objCType], @encode(CGSize)) == 0)
        cg_size = [sizeValue sizeValue];
  }
  CFRelease(size_value);
  return std::make_unique<core::Size>(
      static_cast<int>(cg_size.width), 
      static_cast<int>(cg_size.height));
}

py::array_t<unsigned char> WindowMacOS::get_screenshot() {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
  CFTypeRef window_id_value = nullptr;
  AXError error = AXUIElementCopyAttributeValue(
      window_ref_, CFSTR("AXWindowIdentifier"), &window_id_value);
  handle_error(error);
  CGWindowID window_id = 0;
  if (CFGetTypeID(window_id_value) == CFNumberGetTypeID()) {
    CFNumberGetValue(reinterpret_cast<CFNumberRef>(window_id_value), 
                     kCFNumberIntType, &window_id);
  }
  CFRelease(window_id_value);
  handle_error(error);
  CGImageRef screenshot = CGWindowListCreateImage(
      CGRectNull, kCGWindowListOptionIncludingWindow, 
      window_id, kCGWindowImageDefault);
  if (!screenshot) throw core::exceptions::WindowDoesNotValidException();
  size_t width = CGImageGetWidth(screenshot);
  size_t height = CGImageGetHeight(screenshot);
  CGColorSpaceRef color_space = CGImageGetColorSpace(screenshot);
  if (!color_space) {
    CGImageRelease(screenshot);
    throw core::exceptions::WindowDoesNotValidException();
  }
  std::vector<py::ssize_t> shape = {
    static_cast<py::ssize_t>(height), 
    static_cast<py::ssize_t>(width), 
    4
  };
  py::array_t<unsigned char> image(shape);
  auto buffer = image.mutable_unchecked<3>();
  CGContextRef context = CGBitmapContextCreate(
      buffer.mutable_data(0, 0, 0), width, height, 8, width * 4, 
      color_space, kCGImageAlphaPremultipliedLast);
  if (!context) {
    CGImageRelease(screenshot);
    throw core::exceptions::WindowDoesNotValidException();
  }
  CGContextDrawImage(context, CGRectMake(0, 0, width, height), screenshot);
  CGContextRelease(context);
  CGImageRelease(screenshot);
  return image;
}


std::unique_ptr<int> WindowMacOS::get_process_id() const {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
  pid_t pid = 0;
  AXError error = AXUIElementGetPid(window_ref_, &pid);
  handle_error(error);
  return std::make_unique<int>(static_cast<int>(pid));
}

std::unique_ptr<core::Size> WindowMacOS::get_min_size() const {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
  CFTypeRef min_size_value = nullptr;
  AXError error = AXUIElementCopyAttributeValue(
      window_ref_, CFSTR("AXMinSize"), &min_size_value);
  handle_error(error);
  CGSize cg_size = {0, 0};
  if (min_size_value && CFGetTypeID(min_size_value) == AXValueGetTypeID()) {
    NSValue* sizeValue = (__bridge NSValue*)min_size_value;
    if ([sizeValue isKindOfClass:[NSValue class]])
      if (strcmp([sizeValue objCType], @encode(CGSize)) == 0)
        cg_size = [sizeValue sizeValue];
  }
  CFRelease(min_size_value);
  return std::make_unique<core::Size>(
      static_cast<int>(cg_size.width), 
      static_cast<int>(cg_size.height));
}

std::unique_ptr<core::Size> WindowMacOS::get_max_size() const {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
  CFTypeRef max_size_value = nullptr;
  AXError error = AXUIElementCopyAttributeValue(
      window_ref_, CFSTR("AXMaxSize"), &max_size_value);
  handle_error(error);
  CGSize cg_size = {0, 0};
  if (max_size_value && CFGetTypeID(max_size_value) == AXValueGetTypeID()) {
    NSValue* sizeValue = (__bridge NSValue*)max_size_value;
    if ([sizeValue isKindOfClass:[NSValue class]])
      if (strcmp([sizeValue objCType], @encode(CGSize)) == 0) 
        cg_size = [sizeValue sizeValue];
  }
  CFRelease(max_size_value);
  return std::make_unique<core::Size>(
      static_cast<int>(cg_size.width), 
      static_cast<int>(cg_size.height));
}

}  // namespace appwindows::macos