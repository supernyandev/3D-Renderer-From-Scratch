#pragma once

#include "Mesh.h"
#include <iostream>
namespace renderer {
class BasicObject {
public:
    BasicObject(BasicObject &&) = default;
    BasicObject(const BasicObject &) = default;
    BasicObject() = default;
    void AddTriangle(const Eigen::Matrix3d &);
    const Mesh &GetMesh() const {
        return mesh_;
    }

private:
    Mesh mesh_;
};
}  // namespace renderer
