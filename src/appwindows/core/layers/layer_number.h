#pragma once

#include "layer.h"

namespace appwindows {
namespace core {
namespace layers {

class LayerNumber final : public Layer {
 public:
  explicit LayerNumber(int layer_number);
  int get_layer(int count_of_layers) override;

 private:
  int layer_number_;
};

}  // namespace layers
}  // namespace core
}  // namespace appwindows