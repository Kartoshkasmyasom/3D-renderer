#include "World.hpp"

#include <utility>

namespace Renderer {

void World::add_object(TriangulatedObject&& object) {
  if (stored_objects_.empty()) {
    selected_ = 0;
  }
  stored_objects_.emplace_back(std::move(object));
}

bool World::empty() const {
  return stored_objects_.empty();
}

TriangulatedObject* World::selected_object() {
  if (selected_ < 0) {
    return nullptr;
  }
  return &stored_objects_[selected_];
}

void World::remove_selected_object() {
  if (selected_ == -1) {
    return;
  }

  if (stored_objects_.size() == 1) {
    stored_objects_.clear();
    selected_ = -1;
    return;
  }

  stored_objects_.erase(stored_objects_.begin() + selected_);
  selected_ = selected_ % static_cast<int>(stored_objects_.size());
}

void World::select_next_object() {
  if (selected_ != -1) {
    selected_ = (selected_ + 1) % static_cast<int>(stored_objects_.size());
  }
}

void World::select_previous_object() {
  if (selected_ != -1) {
    selected_ = (selected_ - 1 + static_cast<int>(stored_objects_.size())) %
                static_cast<int>(stored_objects_.size());
  }
}

World::iterator World::begin() {
  return stored_objects_.begin();
}

World::iterator World::end() {
  return stored_objects_.end();
}

World::const_iterator World::begin() const {
  return stored_objects_.begin();
}

World::const_iterator World::end() const {
  return stored_objects_.end();
}

World::~World() noexcept = default;

}  // namespace Renderer
