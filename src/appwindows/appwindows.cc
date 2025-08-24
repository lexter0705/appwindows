#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "./core/platform.h"
#include "core/bind.h"
#include "core/geometry/bind.h"

namespace py = pybind11;

PYBIND11_MODULE(appwindows, m) {
  m.doc() =
      "Python bindings for appwindows library\n\n"
      "The appwindows library solves the problem of working with a graphical "
      "shell on different operating systems.";

  appwindows::core::bind_geometry(m);
  appwindows::core::bind_exceptions(m);
  appwindows::core::bind_window(m);
  appwindows::core::bind_finder(m);

  auto geometry = m.def_submodule("geometry", "Geometry types and operations");
  auto exceptions = m.def_submodule("exceptions", "Exception types");

  geometry.attr("Point") = m.attr("Point");
  geometry.attr("Size") = m.attr("Size");
  exceptions.attr("WindowDoesNotExistError") = m.attr("WindowDoesNotExistError");
  exceptions.attr("InvalidSizeError") = m.attr("InvalidSizeError");

  m.attr("Point").cast<py::object>().dec_ref();
  m.attr("Size").cast<py::object>().dec_ref();
  m.attr("WindowDoesNotExistError").cast<py::object>().dec_ref();
  m.attr("InvalidSizeError").cast<py::object>().dec_ref();

  m.def("get_finder", &appwindows::get_finder,
        "Get the window finder instance\n\n"
        "Returns:\n"
        "    Finder: Instance of window finder interface");
}
