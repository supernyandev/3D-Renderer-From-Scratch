#pragma once

#include <Eigen/Dense>
#include "Vertex.h"
#include "Primitives.h"
#include <array>
namespace renderer {
class Camera {
public:
    Camera(int width, int height);
    Eigen::Matrix<double, 3, 4> ApplyPerspectiveTransformation(
        const Eigen::Matrix<double, 3, 4> &) const;

private:
    void InitPerspective();

    static constexpr double kDefaultNearPlaneDistance = 2;

    double r_, l_, t_, b_;  // not sure what are these
    Eigen::Matrix4d perspective_matrix_ = Eigen::Matrix4d::Zero();
    double near_plane_distance_ = kDefaultNearPlaneDistance;
    // double far_plane_distance_ = 100; idk if needed
};
}  // namespace renderer
