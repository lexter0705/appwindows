#pragma once

#include <memory>

#include "finder.h"

namespace appwindows {

std::unique_ptr<core::Finder> get_finder();

}  // namespace appwindows