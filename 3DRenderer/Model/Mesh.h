#pragma once

#include <Eigen/Dense>
#include <vector>
#include "Primitives.h"
#include "Vertex.h"

namespace renderer {
class Mesh {
    using VerticesConstIterator = decltype(std::declval<const std::vector<Vertex>>().begin());
    using TrianglesConstIterator = decltype(std::declval<const std::vector<Triangle>>().begin());
public:
    Iterable<VerticesConstIterator> GetVerticesIterable() const;

    Iterable<TrianglesConstIterator> GetTrianglesIterable() const;

private:
    std::vector<Vertex> vertices_;
    std::vector<Triangle> triangles_;
};
}  // namespace renderer
