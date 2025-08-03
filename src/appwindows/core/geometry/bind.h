#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace appwindows {
namespace core {

void bind_point(const py::module &m);
void bind_size(const py::module &m);
void bind_geometry(py::module &m);

}  // namespace core
}  // namespace appwindows
