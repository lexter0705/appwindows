#pragma once

#include <memory>

namespace appwindows {
namespace core {
class FinderI;
}  // namespace core

std::unique_ptr<core::FinderI> get_finder();

}  // namespace appwindows