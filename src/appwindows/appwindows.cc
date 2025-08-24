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
  auto geometry = m.def_submodule("geometry", "Geometry types and operations");
  auto exceptions = m.def_submodule("exceptions", "Exception types");

  appwindows::core::bind_geometry(geometry);
  appwindows::core::bind_exceptions(exceptions);

  appwindows::core::bind_window(m);
  appwindows::core::bind_finder(m);

  m.def("get_finder", &appwindows::get_finder,
        "Get the window finder instance\n\n"
        "Returns:\n"
        "    Finder: Instance of window finder interface");
}
