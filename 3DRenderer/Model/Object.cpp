#include "Object.h"
void renderer::BasicObject::AddTriangle(const Eigen::Matrix3d& coordinates) {
    mesh_.AddTriangle(coordinates);
}
