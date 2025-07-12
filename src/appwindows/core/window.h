#pragma once

#include <memory>
#include <string>
#include <vector>

#include "geometry/point.h"
#include "geometry/size.h"

namespace appwindows {
namespace core {

class InstructionI;

class WindowI {
 public:
  virtual ~WindowI() = default;
  virtual std::unique_ptr<std::vector<Point>> get_points() = 0;
  [[nodiscard]] virtual std::unique_ptr<std::string> get_title() const = 0;
  [[nodiscard]] virtual std::unique_ptr<Size> get_size() const = 0;

  virtual void set_active(bool active) = 0;
  virtual void set_maximize(bool is_maximize) = 0;
  virtual void set_size(Size size) = 0;
  virtual void move(Point point) = 0;
  virtual void close() = 0;
};

}  // namespace core
}  // namespace appwindows
