#pragma once

#include "finder.h"

namespace appwindows {
namespace core {

std::vector<std::unique_ptr<std::string>> Finder::get_all_titles() const {
  std::vector<std::unique_ptr<std::string>> titles;
  for (const auto& i : get_all_windows())
    titles.push_back(std::make_unique<std::string>(*i->get_title()));
  return titles;
}

}  // namespace core
}  // namespace appwindows