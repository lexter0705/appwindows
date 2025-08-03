#pragma once

#include "layer.h"

namespace appwindows {
namespace core {
namespace layers {

class Foreground final : public Layer {
public:
  Foreground();
  int get_layer(int count_of_layers) override;
};

}  // namespace layers
}  // namespace core
}  // namespace appwindows