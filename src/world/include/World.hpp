#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>

#include "Camera.hpp"
#include "Lighting.hpp"
#include "object/TriangulatedObject.hpp"

namespace Renderer {

enum class Action;

template <Action action>
struct Command;

class World {
 public:
  using iterator = std::vector<TriangulatedObject>::iterator;
  using const_iterator = std::vector<TriangulatedObject>::const_iterator;

  explicit World();
  ~World() noexcept;

  bool empty() const;
  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  TriangulatedObject* selected_object();

  Matrix4 camera_view_matrix() const;
  Matrix4 camera_projection_matrix(float width, float height) const;
  Vector3 directional_light_direction() const;
  Vector3 directional_light_color() const;
  Vector3 ambient_light_color() const;
  Vector3 world_up() const;

 private:
  void add_object(TriangulatedObject&& object);
  void remove_selected_object();

  void select_next_object();
  void select_previous_object();

  void move_camera_forward(float delta_time);
  void move_camera_backward(float delta_time);
  void move_camera_right(float delta_time);
  void move_camera_left(float delta_time);
  void rotate_camera_left(float delta_time);
  void rotate_camera_right(float delta_time);
  void rotate_camera_up(float delta_time);
  void rotate_camera_down(float delta_time);

  template <Action action>
  friend struct Command;

  int selected_ = -1;
  const Vector3 world_up_{0.0f, 1.0f, 0.0f};
  Camera camera_;
  Lighting light_;
  std::vector<TriangulatedObject> stored_objects_;
};
}  // namespace Renderer
