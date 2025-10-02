#pragma once

#include "finder.h"

namespace appwindows {
namespace core {

std::vector<std::string> Finder::get_all_titles() const {
  std::vector<std::string> titles;
  for (const auto& i : get_all_windows())
    titles.push_back(*i->get_title());
  return titles;
}

}  // namespace core
}  // namespace appwindows