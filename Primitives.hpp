#pragma once

#include <Eigen/Dense>

namespace Renderer{
    class Triangle{
        using Vector3 = Eigen::Vector3d;
        using Vector4 = Eigen::Vector4d;

    public:
        Triangle(Vector3 point0, Vector3 point1, Vector3 point2)
            : point0_(point0), point1_(point1), point2_(point2) {
            }
        
    private:
        Vector3 point0_;
        Vector3 point1_;
        Vector3 point2_;
    };

    class Segment {
        using Vector3 = Eigen::Vector3d;

    public: 
        Segment(Vector3 point0, Vector3 point1) : point0_(point0), point1_(point1) {}

    private:
        Vector3 point0_;
        Vector3 point1_;
    }
} // namespace Renderer