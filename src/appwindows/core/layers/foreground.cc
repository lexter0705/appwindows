#include "foreground.h"

namespace appwindows {
namespace core {
namespace layers {

Foreground::Foreground() = default;

int Foreground::get_layer(const int count_of_layers) {
  return count_of_layers - 1;
};

}  // namespace layers
}  // namespace core
}  // namespace appwindows