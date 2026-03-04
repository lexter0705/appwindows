#include "window.h"

#include <memory>
#include <vector>

#include <CoreGraphics/CGWindow.h>
#include <ScreenCaptureKit/ScreenCaptureKit.h>
#include <dispatch/dispatch.h>

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
  AXError position_error = AXUIElementCopyAttributeValue(
      window_ref_, CFSTR("AXPosition"), &position_value);
  if (position_error != kAXErrorSuccess) {
    if (position_value) CFRelease(position_value);
    if (position_error != kAXErrorSuccess) handle_error(position_error);
  }
  CGPoint position = {0, 0};
  auto size = get_size();
  if (position_value && CFGetTypeID(position_value) == AXValueGetTypeID())
    AXValueGetValue(static_cast<AXValueRef>(position_value),
                    static_cast<AXValueType>(kAXValueCGPointType),
                    &position);
  CFRelease(position_value);
  return std::make_unique<core::QuadPoints>(
			core::Point(static_cast<int>(position.x),
        				static_cast<int>(position.y)),
			core::Point(static_cast<int>(position.x + size->get_width()),
            			static_cast<int>(position.y)),
			core::Point(static_cast<int>(position.x + size->get_width()),
            			static_cast<int>(position.y + size->get_height())),
			core::Point(static_cast<int>(position.x),
            			static_cast<int>(position.y + size->get_height()))
        );
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
  if (size_value) {
    if (CFGetTypeID(size_value) == AXValueGetTypeID())
      AXValueGetValue(static_cast<AXValueRef>(size_value),
                      static_cast<AXValueType>(kAXValueCGSizeType), &cg_size);
    CFRelease(size_value);
  }
  return std::make_unique<core::Size>(
      static_cast<int>(cg_size.width),
      static_cast<int>(cg_size.height));
}

py::array_t<unsigned char> WindowMacOS::get_screenshot() {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();

  pid_t pid = 0;
  AXError pid_error = AXUIElementGetPid(window_ref_, &pid);
  handle_error(pid_error);

  CFArrayRef window_list = CGWindowListCopyWindowInfo(
      kCGWindowListOptionAll | kCGWindowListOptionOnScreenOnly,
      kCGNullWindowID);
  if (!window_list)
    throw core::exceptions::WindowDoesNotValidException("Failed to get window list");

  CGWindowID target_window_id = 0;
  CFIndex window_count = CFArrayGetCount(window_list);
  for (CFIndex i = 0; i < window_count; ++i) {
    CFDictionaryRef window_info = (CFDictionaryRef)CFArrayGetValueAtIndex(window_list, i);
    if (!window_info) continue;

    CFNumberRef window_pid_ref = (CFNumberRef)CFDictionaryGetValue(window_info, kCGWindowOwnerPID);
    pid_t window_pid = 0;
    if (window_pid_ref && CFNumberGetValue(window_pid_ref, kCFNumberIntType, &window_pid)) {
      if (window_pid == pid) {
        CFNumberRef window_id_ref = (CFNumberRef)CFDictionaryGetValue(window_info, kCGWindowNumber);
        if (window_id_ref) {
          CFNumberGetValue(window_id_ref, kCFNumberIntType, &target_window_id);
          break;
        }
      }
    }
  }
  CFRelease(window_list);
  if (target_window_id == 0)
    throw core::exceptions::WindowDoesNotValidException("No window found with this process ID");

  CGRect window_rect = CGRectNull;
  CGWindowImageOption image_options = kCGWindowImageDefault | kCGWindowImageBestResolution;
  CGImageRef cg_image = CGWindowListCreateImage(window_rect,
                                                kCGWindowListOptionIncludingWindow,
                                                target_window_id,
                                                image_options);
  if (!cg_image)
    throw core::exceptions::WindowDoesNotValidException("Failed to capture window image");

  size_t width = CGImageGetWidth(cg_image);
  size_t height = CGImageGetHeight(cg_image);
  CGColorSpaceRef color_space = CGColorSpaceCreateDeviceRGB();
  size_t bytes_per_row = width * 4;
  std::vector<unsigned char> buffer(bytes_per_row * height);
  CGContextRef context = CGBitmapContextCreate(
      buffer.data(), width, height, 8, bytes_per_row, color_space,
      kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Little);

  if (!context) {
    CGColorSpaceRelease(color_space);
    CGImageRelease(cg_image);
    throw core::exceptions::WindowDoesNotValidException("Failed to create bitmap context");
  }

  CGContextDrawImage(context, CGRectMake(0, 0, width, height), cg_image);
  CGContextRelease(context);
  CGColorSpaceRelease(color_space);
  CGImageRelease(cg_image);

  std::vector<py::ssize_t> shape = {static_cast<py::ssize_t>(height),
                                    static_cast<py::ssize_t>(width),
                                    3};
  py::array_t<unsigned char> image(shape);
  auto image_buffer = image.mutable_unchecked<3>();

  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      size_t src_index = y * bytes_per_row + x * 4;
      unsigned char blue = buffer[src_index];
      unsigned char green = buffer[src_index + 1];
      unsigned char red = buffer[src_index + 2];
      image_buffer(y, x, 0) = red;
      image_buffer(y, x, 1) = green;
      image_buffer(y, x, 2) = blue;
    }
  }

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