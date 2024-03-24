#pragma once
#include "Primitives.h"
#include "Vertex.h"
#include <Eigen/Dense>
#include <vector>
namespace renderer {
class Mesh {
public:
    using Vector3i = Eigen::Vector3i;
    struct ITriangle {
        Vector3i points_;
    };
    using VerticesConstIterator = std::vector<Vertex>::const_iterator;
    using TrianglesConstIterator = std::vector<ITriangle>::const_iterator;
    Iterable<VerticesConstIterator> GetVertices() const;

    Iterable<TrianglesConstIterator> GetTriangles() const;

    Triangle GetTriangleVertices(const ITriangle &) const;
    void AddTriangle(const Eigen::Matrix3d &);

private:
    std::vector<Vertex> vertices_;
    std::vector<ITriangle> triangles_;
};
}  // namespace renderer
