#include "bind.h"

#include <pybind11/pybind11.h>

#include "point.h"
#include "size.h"

namespace py = pybind11;

namespace appwindows {
namespace core {

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
      .def_property_readonly("x", &Point::get_x, "X coordinate")
      .def_property_readonly("y", &Point::get_y, "Y coordinate");
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
      .def_property_readonly("width", &Size::get_width, "Width dimension")
      .def_property_readonly("height", &Size::get_height, "Height dimension");
}

void bind_geometry(py::module &m) {
  bind_size(m);
  bind_point(m);
}

}  // namespace core
}  // namespace appwindows
