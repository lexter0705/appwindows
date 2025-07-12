#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "./core/exceptions/invalid_size.h"
#include "./core/exceptions/window_does_not_exist.h"
#include "./core/finder.h"
#include "./core/platform.h"
#include "./core/window.h"

namespace py = pybind11;

namespace appwindows {

void bind_exceptions(py::module &m) {
  static py::exception<core::exceptions::WindowDoesNotExistException>
      window_exc(m, "WindowDoesNotExistError");
  static py::exception<core::exceptions::InvalidSizeException> size_exc(
      m, "InvalidSizeError");

  py::register_exception_translator([](std::exception_ptr p) {
    try {
      if (p) std::rethrow_exception(p);
    } catch (const core::exceptions::WindowDoesNotExistException &e) {
      window_exc(e.what());
    } catch (const core::exceptions::InvalidSizeException &e) {
      size_exc(e.what());
    }
  });
}

void bind_point(py::module &m) {
  py::class_<core::Point>(m, "Point",
                          "Represents a 2D point with x and y coordinates")
      .def(py::init<>(), "Create a point at origin (0, 0)")
      .def(py::init<int, int>(),
           "Create a point with specified coordinates\n\n"
           "Args:\n"
           "    x (int): X coordinate\n"
           "    y (int): Y coordinate",
           py::arg("x"), py::arg("y"))
      .def("__add__", &core::Point::operator+, "Add two points")
      .def("__sub__", &core::Point::operator-, "Subtract two points")
      .def("__mul__", &core::Point::operator*, "Multiply two points")
      .def("__truediv__", &core::Point::operator/, "Divide two points")
      .def_property_readonly("x", &core::Point::getX, "X coordinate")
      .def_property_readonly("y", &core::Point::getY, "Y coordinate");
}

void bind_size(py::module &m) {
  py::class_<core::Size>(m, "Size",
                         "Represents dimensions with width and height")
      .def(py::init<int, int>(),
           "Create size with specified dimensions\n\n"
           "Args:\n"
           "    width (int): Width dimension\n"
           "    height (int): Height dimension\n\n"
           "Raises:\n"
           "    InvalidSizeError: If width or height are invalid",
           py::arg("width"), py::arg("height"))
      .def_property_readonly("width", &core::Size::getWidth, "Width dimension")
      .def_property_readonly("height", &core::Size::getHeight,
                             "Height dimension");
}

void bind_window(py::module &m) {
  py::class_<core::WindowI, std::shared_ptr<core::WindowI>>(
      m, "WindowI", "Interface representing an application window")
      .def(
          "get_points",
          [](core::WindowI &self) {
            auto points = self.get_points();
            return points ? *points : std::vector<core::Point>{};
          },
          "Get all points associated with the window\n\n"
          "Returns:\n"
          "    list[Point]: List of points")
      .def(
          "get_title",
          [](core::WindowI &self) {
            auto title = self.get_title();
            return title ? *title : std::string{};
          },
          "Get the window title\n\n"
          "Returns:\n"
          "    str: Window title")
      .def(
          "get_size",
          [](core::WindowI &self) {
            auto size = self.get_size();
            return size ? *size : core::Size{0, 0};
          },
          "Get current window size\n\n"
          "Returns:\n"
          "    Size: Current window dimensions")
      .def("set_active", &core::WindowI::set_active,
           "Set window active state\n\n"
           "Args:\n"
           "    active (bool): True to activate window",
           py::arg("active"))
      .def("set_maximize", &core::WindowI::set_maximize,
           "Maximize or restore the window\n\n"
           "Args:\n"
           "    is_maximize (bool): True to maximize window",
           py::arg("is_maximize"))
      .def("set_size", &core::WindowI::set_size,
           "Resize the window\n\n"
           "Args:\n"
           "    size (Size): New window dimensions\n\n"
           "Raises:\n"
           "    InvalidSizeError: If size is invalid",
           py::arg("size"))
      .def("move", &core::WindowI::move,
           "Move window to specified position\n\n"
           "Args:\n"
           "    point (Point): New window position",
           py::arg("point"))
      .def("close", &core::WindowI::close, "Close the window");
}

void bind_finder(py::module &m) {
  py::class_<core::FinderI, std::shared_ptr<core::FinderI>>(
      m, "FinderI", "Interface for finding application windows")
      .def(
          "get_window_by_title",
          [](const core::FinderI &self, const std::string &title) {
            auto window = self.get_window_by_title(title);
            if (!window) {
              throw core::exceptions::WindowDoesNotExistException();
            }
            return window;
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

}  // namespace appwindows

PYBIND11_MODULE(appwindows, m) {
  m.doc() =
      "Python bindings for appwindows library\n\n"
      "The appwindows library solves the problem of working with a graphical "
      "shell on different operating systems.";
  appwindows::bind_exceptions(m);
  auto geometry = m.def_submodule("geometry", "Geometry types and operations");
  appwindows::bind_point(geometry);
  appwindows::bind_size(geometry);
  appwindows::bind_window(m);
  appwindows::bind_finder(m);
  m.def("get_finder", &appwindows::get_finder,
        "Get the window finder instance\n\n"
        "Returns:\n"
        "    FinderI: Instance of window finder interface");
}
