#pragma once

#include <Eigen/Dense>

namespace Renderer{
using Vector3 = Eigen::Vector3f;
using Matrix4 = Eigen::Matrix4f;

class Camera {
public:
    Camera(
        const Vector3& position = Vector3(0, 0, 3),
        const Vector3& target   = Vector3(0, 0, 0),
        const Vector3& up       = Vector3(0, 1, 0),
        float fov_degrees = 45.0f,
        float z_near = 0.1f,
        float z_far = 100.0f
    ): position_(position),
          target_(target),
          world_up_(up),
          fov_degrees_(fov_degrees),
          z_near_(z_near),
          z_far_(z_far)
    {
        update_vectors();
    }

    Matrix4 view_matrix() const;
    Matrix4 projection_matrix(float width, float height) const;
    
    void set_position(const Vector3& position);
    void set_target(const Vector3& target);

private:
    void update_vectors();

private:
    Vector3 position_;
    Vector3 target_;

    Vector3 world_up_;   

    Vector3 direction_; 
    Vector3 camera_right_;      
    Vector3 camera_up_;         

    float fov_degrees_;
    float z_near_;
    float z_far_;
};

} // namespace Renderer
