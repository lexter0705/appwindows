#include "bind.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "exceptions/invalid_size.h"
#include "exceptions/window_does_not_exist.h"
#include "finder.h"
#include "geometry/bind.h"
#include "geometry/point.h"
#include "geometry/size.h"
#include "window.h"

namespace py = pybind11;

namespace appwindows {
namespace core {

void bind_exceptions(const py::module &m) {
  static py::exception<exceptions::WindowDoesNotExistException>
      WindowDoesNotExistException(m, "WindowDoesNotExistError");
  static py::exception<exceptions::InvalidSizeException> InvalidSizeException(
      m, "InvalidSizeError");

  py::register_exception_translator([](std::exception_ptr p) {
    try {
      if (p) std::rethrow_exception(p);
    } catch (const exceptions::WindowDoesNotExistException &e) {
      WindowDoesNotExistException(e.what());
    } catch (const exceptions::InvalidSizeException &e) {
      InvalidSizeException(e.what());
    }
  });
}

void bind_window(py::module &m) {
  py::class_<Window, std::shared_ptr<Window>>(
      m, "Window", "Interface representing an application window")
      .def(
          "get_points",
          [](Window &self) {
            auto points = self.get_points();
            return points ? *points : std::vector<Point>{};
          },
          "Get all points associated with the window\n\n"
          "Returns:\n"
          "    list[Point]: List of points")
      .def(
          "get_title",
          [](const Window &self) {
            const auto title = self.get_title();
            return title ? *title : std::string{};
          },
          "Get the window title\n\n"
          "Returns:\n"
          "    str: Window title")
      .def(
          "get_size",
          [](const Window &self) {
            auto size = self.get_size();
            return size ? *size : Size{0, 0};
          },
          "Get current window size\n\n"
          "Returns:\n"
          "    Size: Current window dimensions")
      .def("set_active", &Window::set_active,
           "Set window active state\n\n"
           "Args:\n"
           "    active (bool): True to activate window",
           py::arg("active"))
      .def("set_maximize", &Window::set_maximize,
           "Maximize or restore the window\n\n"
           "Args:\n"
           "    is_maximize (bool): True to maximize window",
           py::arg("is_maximize"))
      .def("resize", &Window::resize,
           "Resize the window\n\n"
           "Args:\n"
           "    size (Size): New window dimensions\n\n"
           "Raises:\n"
           "    InvalidSizeError: If size is invalid",
           py::arg("size"))
      .def("move", &Window::move,
           "Move window to specified position\n\n"
           "Args:\n"
           "    point (Point): New window position",
           py::arg("point"))
      .def("close", &Window::close, "Close the window");
}

void bind_finder(const py::module &m) {
  py::class_<Finder, std::shared_ptr<Finder>>(
      m, "Finder", "Interface for finding application windows")
      .def(
          "get_window_by_title",
          [](const Finder &self, const std::string &title) {
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
          [](const Finder &self) { return self.get_all_windows(); },
          "Find all opened windows\n\n"
          "Returns:\n"
          "    list[Window]: Found windows\n\n");
  ;
}

void bind_core(py::module &m) {
  bind_window(m);
  const auto exceptions = m.def_submodule("exceptions", "Exception types");
  bind_geometry(m);
  bind_exceptions(exceptions);
  bind_finder(m);
}

}  // namespace core
}  // namespace appwindows
