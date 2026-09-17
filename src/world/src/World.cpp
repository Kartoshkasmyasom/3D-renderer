#include "World.hpp"

#include <utility>

namespace Renderer {

World::World()
    : camera_(Vector3(0.0f, 0.0f, 3.0f), -90.0f, 0.0f, 45.0f, 0.1f, 100.0f,
              3.0f, 90.0f, world_up_) {}

void World::add_object(TriangulatedObject&& object) {
  if (stored_objects_.empty()) {
    selected_ = 0;
  }
  stored_objects_.emplace_back(std::move(object));
}

bool World::empty() const { return stored_objects_.empty(); }

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

World::iterator World::begin() { return stored_objects_.begin(); }

World::iterator World::end() { return stored_objects_.end(); }

World::const_iterator World::begin() const { return stored_objects_.begin(); }

World::const_iterator World::end() const { return stored_objects_.end(); }

Matrix4 World::camera_view_matrix() const { return camera_.view_matrix(); }

Matrix4 World::camera_projection_matrix(float width, float height) const {
  return camera_.projection_matrix(width, height);
}

Vector3 World::directional_light_direction() const {
  return light_.directional_light().direction;
}

Vector3 World::directional_light_color() const {
  return light_.directional_light().color;
}

Vector3 World::ambient_light_color() const { return light_.ambient_light(); }

Vector3 World::world_up() const { return world_up_; }

void World::move_camera_forward(float delta_time) {
  camera_.move_forward(delta_time);
}

void World::move_camera_backward(float delta_time) {
  camera_.move_backward(delta_time);
}

void World::move_camera_right(float delta_time) {
  camera_.move_right(delta_time);
}

void World::move_camera_left(float delta_time) {
  camera_.move_left(delta_time);
}

void World::rotate_camera_left(float delta_time) {
  camera_.rotate_left(delta_time);
}

void World::rotate_camera_right(float delta_time) {
  camera_.rotate_right(delta_time);
}

void World::rotate_camera_up(float delta_time) {
  camera_.rotate_up(delta_time);
}

void World::rotate_camera_down(float delta_time) {
  camera_.rotate_down(delta_time);
}

World::~World() noexcept = default;

}  // namespace Renderer
