#pragma once

#include "layer.h"

namespace appwindows {
namespace core {
namespace layers {

class Background final : public Layer {
public:
  Background();
  int get_layer(int count_of_layers) override;
};

}  // namespace layers
}  // namespace core
}  // namespace appwindows