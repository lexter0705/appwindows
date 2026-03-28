#include "size.h"

#include "../exceptions/invalid_size.h"

namespace appwindows::core {

bool Size::operator==(const Size& other) const {
  return get_width() == other.get_width() && get_height() == other.get_height();
}

bool Size::operator!=(const Size& other) const {
  return get_width() != other.get_width() || get_height() != other.get_height();
}

}  // namespace appwindows::core