#ifdef _WIN32 || _WIN64

#include "bind.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "finder.h"
#include "window.h"

namespace py = pybind11;

namespace appwindows {
namespace windows {

void bind_window(const py::module &m) {
  py::class_<WindowWindows, std::shared_ptr<WindowWindows>>(
      m, "WindowWindows",
      "Class representing an application window in Windows OS")
      .def(
          "get_points",
          [](WindowWindows &self) {
            const auto points = self.get_points();
            return points ? *points : std::vector<core::Point>{};
          },
          "Get all points associated with the window\n\n"
          "Returns:\n"
          "    list[Point]: List of points")
      .def(
          "get_title",
          [](const WindowWindows &self) {
            const auto title = self.get_title();
            return title ? *title : std::string{};
          },
          "Get the window title\n\n"
          "Returns:\n"
          "    str: Window title")
      .def(
          "get_size",
          [](const WindowWindows &self) {
            const auto size = self.get_size();
            return size ? *size : core::Size{0, 0};
          },
          "Get current window size\n\n"
          "Returns:\n"
          "    Size: Current window dimensions")
      .def("set_active", &WindowWindows::set_active,
           "Set window active state\n\n"
           "Args:\n"
           "    active (bool): True to activate window",
           py::arg("active"))
      .def("set_maximize", &WindowWindows::set_maximize,
           "Maximize or restore the window\n\n"
           "Args:\n"
           "    is_maximize (bool): True to maximize window",
           py::arg("is_maximize"))
      .def("set_size", &WindowWindows::set_size,
           "Resize the window\n\n"
           "Args:\n"
           "    size (Size): New window dimensions\n\n"
           "Raises:\n"
           "    InvalidSizeError: If size is invalid",
           py::arg("size"))
      .def("move", &WindowWindows::move,
           "Move window to specified position\n\n"
           "Args:\n"
           "    point (Point): New window position",
           py::arg("point"))
      .def("close", &WindowWindows::close, "Close the window");
}

void bind_finder(const py::module &m) {
  py::class_<FinderWindows, std::shared_ptr<FinderWindows>>(
      m, "FinderWindows", "Class for finding application windows in Windows OS")
      .def(py::init<>())
      .def(
          "get_window_by_title",
          [](const FinderWindows &self, const std::string &title) {
            return self.get_window_by_title(title);
          },
          "Find window by its title\n\n"
          "Args:\n"
          "    title (str): Window title to search for\n\n"
          "Returns:\n"
          "    WindowI: Found window\n\n"
          "Raises:\n"
          "    WindowDoesNotExistError: If window not found",
          py::arg("title"))
      .def(
          "get_all_windows",
          [](const FinderWindows &self) { return self.get_all_windows(); },
          "Find all opened windows\n\n"
          "Returns:\n"
          "    list[Window]: Found window\n\n");
}

void bind_windows(py::module &m) {
  const auto windows =
      m.def_submodule("windows", "Windows types and operations");
  bind_finder(windows);
  bind_window(windows);
}

}  // namespace windows
}  // namespace appwindows

#endif