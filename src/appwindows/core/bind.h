#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace appwindows {
namespace core {

void bind_window(const py::module &m);
void bind_finder(const py::module &m);
void bind_exceptions(const py::module &m);
void bind_core(py::module &m, py::module &geometry, py::module &exceptions);

}  // namespace windows
}  // namespace appwindows
