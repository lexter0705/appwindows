#include "finder.h"

#include <ApplicationServices/ApplicationServices.h>

#include <memory>
#include <vector>

#include "../core/base_window.h"
#include "window.h"

namespace appwindows::macos {

FinderMacOS::FinderMacOS() = default;

std::vector<std::shared_ptr<core::Window>> FinderMacOS::get_all_windows()
    const {
  CFArrayRef window_list = CGWindowListCopyWindowInfo(
      kCGWindowListOptionOnScreenOnly | kCGWindowListExcludeDesktopElements,
      kCGNullWindowID);
  std::vector<std::shared_ptr<core::Window>> windows;
  CFIndex count = CFArrayGetCount(window_list);

  for (CFIndex i = 0; i < count; ++i) {
    CFDictionaryRef window_info = reinterpret_cast<CFDictionaryRef>(
        CFArrayGetValueAtIndex(window_list, i));
    CFNumberRef pid_ref = reinterpret_cast<CFNumberRef>(
        CFDictionaryGetValue(window_info, kCGWindowOwnerPID));
    CFNumberRef window_id_ref = reinterpret_cast<CFNumberRef>(
        CFDictionaryGetValue(window_info, kCGWindowNumber));

    if (!pid_ref || !window_id_ref) continue;

    int pid;
    CFNumberGetValue(pid_ref, kCFNumberIntType, &pid);
    if (pid <= 0) continue;

    CGWindowID window_id;
    CFNumberGetValue(window_id_ref, kCFNumberIntType, &window_id);
    windows.push_back(std::make_shared<WindowMacOS>(window_id));
  }

  CFRelease(window_list);
  return windows;
}

std::unique_ptr<std::string> FinderMacOS::get_os() const {
  return std::make_unique<std::string>("macos::aqua");
}

}  // namespace appwindows::macos