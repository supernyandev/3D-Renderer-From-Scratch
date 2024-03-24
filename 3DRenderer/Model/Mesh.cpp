#include "Mesh.h"

namespace renderer {
Iterable<Mesh::TrianglesConstIterator> Mesh::GetTriangles() const {
    return Iterable(triangles_.begin(), triangles_.end());
}

Iterable<Mesh::VerticesConstIterator> Mesh::GetVertices() const {
    return Iterable(vertices_.begin(), vertices_.end());
}
Triangle Mesh::GetTriangleVertices(const ITriangle& triangle) const {

    Triangle ans;
    for (size_t i = 0; i < 3; ++i) {
        int t = triangle.points_(i);
        ans.GetVerticies()[i] = vertices_[triangle.points_(i)];  // TODO get triangle
    }

    return ans;
}

void Mesh::AddTriangle(const Eigen::Matrix3d& coordinates) {
    Eigen::Vector3d v1 = coordinates.row(1) - coordinates.row(0);
    Eigen::Vector3d v2 = coordinates.row(2) - coordinates.row(0);
    Eigen::Vector3d normal = v1.cross(v2);
    normal.normalize();
    int current_ind = vertices_.size();
    for (int i = 0; i < 3; ++i) {
        vertices_.emplace_back(coordinates.row(i), normal);
    }

    triangles_.push_back(ITriangle{Vector3i{current_ind, current_ind + 1, current_ind + 2}});
}
}  // namespace renderer
