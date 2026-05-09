#pragma once

#include "TriangulatedObject.hpp"

#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>

namespace Renderer {

class World {
 public:
  using iterator = std::vector<TriangulatedObject>::iterator;
  using const_iterator = std::vector<TriangulatedObject>::const_iterator;

  World() = default;
  ~World() noexcept;
  void add_object(TriangulatedObject&& object);
  bool empty() const;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  TriangulatedObject* selected_object();
  void remove_selected_object();
  void select_next_object();
  void select_previous_object();

 private:
  int selected_ = -1;
  std::vector<TriangulatedObject> stored_objects_;
};
}  // namespace Renderer
