#include "bind.h"

#include <pybind11/pybind11.h>

#include "exceptions/invalid_size.h"
#include "exceptions/window_does_not_exist.h"
#include "finder.h"
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

void bind_point(const py::module &m) {
  py::class_<Point>(m, "Point",
                    "Represents a 2D point with x and y coordinates")
      .def(py::init<int, int>(),
           "Create a point with specified coordinates\n\n"
           "Args:\n"
           "    x (int): X coordinate\n"
           "    y (int): Y coordinate",
           py::arg("x"), py::arg("y"))
      .def("__add__", &Point::operator+, "Add two points")
      .def("__sub__", &Point::operator-, "Subtract two points")
      .def("__mul__", &Point::operator*, "Multiply two points")
      .def("__truediv__", &Point::operator/, "Divide two points")
      .def_property_readonly("x", &Point::getX, "X coordinate")
      .def_property_readonly("y", &Point::getY, "Y coordinate");
}

void bind_size(const py::module &m) {
  py::class_<Size>(m, "Size", "Represents dimensions with width and height")
      .def(py::init<int, int>(),
           "Create size with specified dimensions\n\n"
           "Args:\n"
           "    width (int): Width dimension\n"
           "    height (int): Height dimension\n\n"
           "Raises:\n"
           "    InvalidSizeError: If width or height are invalid",
           py::arg("width"), py::arg("height"))
      .def_property_readonly("width", &Size::getWidth, "Width dimension")
      .def_property_readonly("height", &Size::getHeight, "Height dimension");
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
          [](Window &self) {
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
      .def("set_size", &Window::set_size,
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
            auto window = self.get_window_by_title(title);
            return window;
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
          "    list[Window]: Found window\n\n");
  ;
}

void bind_core(py::module &m) {
  bind_window(m);
  const auto exceptions = m.def_submodule("exceptions", "Exception types");
  const auto geometry =
      m.def_submodule("geometry", "Geometry types and operations");
  bind_size(geometry);
  bind_point(geometry);
  bind_exceptions(exceptions);
  bind_finder(m);
}

}  // namespace core
}  // namespace appwindows
