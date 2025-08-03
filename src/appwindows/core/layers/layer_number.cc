#include "layer_number.h"

#include <stdexcept>

namespace appwindows {
namespace core {
namespace layers {

LayerNumber::LayerNumber(const int layer_number) : layer_number_(layer_number) {
  if (layer_number < 0) throw std::invalid_argument("Invalid layer number");
};

int LayerNumber::get_layer(const int count_of_layers) override {
  if (layer_number_ >= count_of_layers) return count_of_layers - 1;
  return layer_number_;
};

}  // namespace layers
}  // namespace core
}  // namespace appwindows