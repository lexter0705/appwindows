#include "bind_exceptions.h"

#include <pybind11/pybind11.h>

#include "invalid_size.h"
#include "window_does_not_valid.h" 
#include "window_does_not_found.h" 
#include "permission_denied.h" 

namespace py = pybind11;

namespace appwindows::core::exceptions {

void bind_exceptions(py::module &m){
  py::register_exception<InvalidSizeException>(m, "InvalidSizeException");
  py::register_exception<WindowDoesNotValidException>(m, "WindowDoesNotValidException");
  py::register_exception<WindowDoesNotFoundException>(m, "WindowDoesNotFoundException");
  py::register_exception<PermissionDeniedException>(m, "WindowDoesNotFoundException");
}

}  // namespace appwindows::core::geometry