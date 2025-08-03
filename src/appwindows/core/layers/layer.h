#pragma once

namespace appwindows {
namespace core {
namespace layers {

class Layer {
 public:
  virtual ~Layer() = default;
  virtual int get_layer(int count_of_layers) = 0;
};

}  // namespace layers
}  // namespace core
}  // namespace appwindows