#pragma once
#ifdef __linux__

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace appwindows {
namespace windows {

void bind_window(const py::module &m);
void bind_finder(const py::module &m);
void bind_x_server(py::module &m);

}  // namespace windows
}  // namespace appwindows
#endif
