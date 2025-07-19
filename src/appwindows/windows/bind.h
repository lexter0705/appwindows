#pragma once

#ifdef _WIN32 || _WIN64

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace appwindows {
namespace windows {

void bind_window(const py::module &m);
void bind_finder(const py::module &m);
void bind_windows(py::module &m);

}  // namespace windows
}  // namespace appwindows
#endif
