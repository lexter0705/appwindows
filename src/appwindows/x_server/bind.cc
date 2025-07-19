#ifdef __linux__
#include "bind.h"

#include <pybind11/pybind11.h>

#include "finder.h"
#include "window.h"

namespace py = pybind11;

namespace appwindows {
namespace x_server {

void bind_window(const py::module &m) {
  py::class_<WindowXServer, std::shared_ptr<WindowXServer>>(
      m, "WindowXServer", "Class representing an application window in XServer")
      .def(
          "get_points",
          [](WindowXServer &self) {
            const auto points = self.get_points();
            return points ? *points : std::vector<core::Point>{};
          },
          "Get all points associated with the window\n\n"
          "Returns:\n"
          "    list[Point]: List of points")
      .def(
          "get_title",
          [](const WindowXServer &self) {
            const auto title = self.get_title();
            return title ? *title : std::string{};
          },
          "Get the window title\n\n"
          "Returns:\n"
          "    str: Window title")
      .def(
          "get_size",
          [](const WindowXServer &self) {
            const auto size = self.get_size();
            return size ? *size : core::Size{0, 0};
          },
          "Get current window size\n\n"
          "Returns:\n"
          "    Size: Current window dimensions")
      .def("set_active", &WindowXServer::set_active,
           "Set window active state\n\n"
           "Args:\n"
           "    active (bool): True to activate window",
           py::arg("active"))
      .def("set_maximize", &WindowXServer::set_maximize,
           "Maximize or restore the window\n\n"
           "Args:\n"
           "    is_maximize (bool): True to maximize window",
           py::arg("is_maximize"))
      .def("set_size", &WindowXServer::set_size,
           "Resize the window\n\n"
           "Args:\n"
           "    size (Size): New window dimensions\n\n"
           "Raises:\n"
           "    InvalidSizeError: If size is invalid",
           py::arg("size"))
      .def("move", &WindowXServer::move,
           "Move window to specified position\n\n"
           "Args:\n"
           "    point (Point): New window position",
           py::arg("point"))
      .def("close", &WindowXServer::close, "Close the window");
}

void bind_finder(const py::module &m) {
  py::class_<FinderXServer, std::shared_ptr<FinderXServer>>(
      m, "FinderXServer", "Class for finding application windows in XServer")
      .def(
          "get_window_by_title",
          [](const FinderXServer &self, const std::string &title) {
            return self.get_window_by_title(title);
          },
          "Find window by its title\n\n"
          "Args:\n"
          "    title (str): Window title to search for\n\n"
          "Returns:\n"
          "    WindowI: Found window\n\n"
          "Raises:\n"
          "    WindowDoesNotExistError: If window not found",
          py::arg("title"));
}

void bind_x_server(py::module &m) {
  const auto x_server =
      m.def_submodule("x_server", "XServer types and operations");
  bind_finder(x_server);
  bind_window(x_server);
}

}  // namespace x_server
}  // namespace appwindows
#endif
