#pragma once

#include <Eigen/Dense>
#include "Utils.h"
namespace renderer {
struct Vertex {
    Vertex(Eigen::Vector3d point, Eigen::Vector3d normal_vec)
        : coordinates(point), normal(normal_vec) {
    }
    Vertex() = default;
    Point coordinates;
    Direction normal;
};

}  // namespace renderer
