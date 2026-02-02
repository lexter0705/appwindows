#include "window.h"

#include <memory>

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <Accessibility/Accessibility.h>

#include "../core/exceptions/window_does_not_valid.h"
#include "../core/geometry/point.h"
#include "../core/geometry/quad_points.h"
#include "../core/geometry/size.h"

namespace appwindows::macos {

static CFStringRef kAXPositionAttribute = CFSTR("AXPosition");
static CFStringRef kAXSizeAttribute = CFSTR("AXSize");
static CFStringRef kAXTitleAttribute = CFSTR("AXTitle");
static CFStringRef kAXWindowIdentifierAttribute = CFSTR("AXWindowIdentifier");
static CFStringRef kAXMinSizeAttribute = CFSTR("AXMinSize");
static CFStringRef kAXMaxSizeAttribute = CFSTR("AXMaxSize");
static CFStringRef kAXWindowNumberAttribute = CFSTR("AXWindowNumber");

WindowMacOS::WindowMacOS(AXUIElementRef window_ref) : window_ref_(window_ref) {
  if (window_ref_) CFRetain(window_ref_);
}

WindowMacOS::~WindowMacOS() {
  if (window_ref_) CFRelease(window_ref_);
}

std::unique_ptr<core::QuadPoints> WindowMacOS::get_points() {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
  CFTypeRef position_value = nullptr;
  CFTypeRef size_value = nullptr;
  AXError position_error = AXUIElementCopyAttributeValue(
      window_ref_, kAXPositionAttribute, &position_value);
  AXError size_error = AXUIElementCopyAttributeValue(
      window_ref_, kAXSizeAttribute, &size_value);
  if (position_error != kAXErrorSuccess || size_error != kAXErrorSuccess) {
    if (position_value) CFRelease(position_value);
    if (size_value) CFRelease(size_value);
    throw core::exceptions::WindowDoesNotValidException();
  }
  CGPoint position = {0, 0};
  CGSize size = {0, 0};
  if (AXValueGetType(position_value) == kAXValueCGPointType) {
    AXValueGetValue(position_value, kAXValueCGPointType, &position);
  }
  if (AXValueGetType(size_value) == kAXValueCGSizeType) {
    AXValueGetValue(size_value, kAXValueCGSizeType, &size);
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
      window_ref_, kAXTitleAttribute, &title_value);
  if (error != kAXErrorSuccess) {
    return std::make_unique<std::string>("");
  }
  CFStringRef title_string = reinterpret_cast<CFStringRef>(title_value);
  CFIndex length = CFStringGetLength(title_string);
  CFIndex max_size = CFStringGetMaximumSizeForEncoding(length,
  char* buffer = new char[max_size];
  std::unique_ptr<std::string> result;
  if (CFStringGetCString(title_string, buffer, max_size, kCFStringEncodingUTF8))
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
      window_ref_, kAXSizeAttribute, &size_value);
  if (error != kAXErrorSuccess)
    throw core::exceptions::WindowDoesNotValidException();
  CGSize cg_size = {0, 0};
  if (AXValueGetType(size_value) == kAXValueCGSizeType) {
    AXValueGetValue(size_value, kAXValueCGSizeType, &cg_size);
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
      window_ref_, kAXWindowIdentifierAttribute, &window_id_value);
  if (error != kAXErrorSuccess)
    throw core::exceptions::WindowDoesNotValidException();
  CGWindowID window_id = 0;
  if (CFGetTypeID(window_id_value) == CFNumberGetTypeID())
    CFNumberGetValue(reinterpret_cast<CFNumberRef>(window_id_value), kCFNumberIntType, &window_id);
  CFRelease(window_id_value);
  if (window_id == 0)
    throw core::exceptions::WindowDoesNotValidException();
  CGImageRef screenshot = CGWindowListCreateImage(
      CGRectNull, kCGWindowListOptionIncludingWindow,
      window_id, kCGWindowImageDefault);
  if (!screenshot)
    throw core::exceptions::WindowDoesNotValidException();
  size_t width = CGImageGetWidth(screenshot);
  size_t height = CGImageGetHeight(screenshot);
  CGColorSpaceRef color_space = CGImageGetColorSpace(screenshot);
  if (!color_space) {
    CGImageRelease(screenshot);
    throw core::exceptions::WindowDoesNotValidException();
  }
  py::array_t<unsigned char> image({static_cast<py::ssize_t>(height),
                                   static_cast<py::ssize_t>(width), 4});
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
  if (error != kAXErrorSuccess)
    throw core::exceptions::WindowDoesNotValidException();
  return std::make_unique<int>(static_cast<int>(pid));
}

std::unique_ptr<bool> WindowMacOS::is_valid() const {
  if (!window_ref_)
    return std::make_unique<bool>(false);
  pid_t pid = 0;
  AXError error = AXUIElementGetPid(window_ref_, &pid);
  if (error != kAXErrorSuccess)
    return std::make_unique<bool>(false);
  CFTypeRef window_id_value = nullptr;
  error = AXUIElementCopyAttributeValue(
      window_ref_, kAXWindowIdentifierAttribute, &window_id_value);
  if (error == kAXErrorSuccess)
    CFRelease(window_id_value);
  return std::make_unique<bool>(error == kAXErrorSuccess);
}

std::unique_ptr<core::Size> WindowMacOS::get_min_size() const {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
  CFTypeRef min_size_value = nullptr;
  AXError error = AXUIElementCopyAttributeValue(
      window_ref_, kAXMinSizeAttribute, &min_size_value);
  if (error != kAXErrorSuccess)
    throw core::exceptions::WindowDoesNotValidException();
  CGSize cg_size = {0, 0};
  if (AXValueGetType(min_size_value) == kAXValueCGSizeType)
    AXValueGetValue(min_size_value, kAXValueCGSizeType, &cg_size);
  CFRelease(min_size_value);
  return std::make_unique<core::Size>(
      static_cast<int>(cg_size.width),
      static_cast<int>(cg_size.height));
}

std::unique_ptr<core::Size> WindowMacOS::get_max_size() const {
  if (!*is_valid()) throw core::exceptions::WindowDoesNotValidException();
  CFTypeRef max_size_value = nullptr;
  AXError error = AXUIElementCopyAttributeValue(
      window_ref_, kAXMaxSizeAttribute, &max_size_value);
  if (error != kAXErrorSuccess)
    throw core::exceptions::WindowDoesNotValidException();
  CGSize cg_size = {0, 0};
  if (AXValueGetType(max_size_value) == kAXValueCGSizeType)
    AXValueGetValue(max_size_value, kAXValueCGSizeType, &cg_size);
  CFRelease(max_size_value);
  return std::make_unique<core::Size>(
      static_cast<int>(cg_size.width),
      static_cast<int>(cg_size.height));
}

}  // namespace appwindows::macos