#include "Primitives.h"
#include <algorithm>
namespace renderer {
Triangle::Triangle(Eigen::Matrix3d coordinates) {

    verticies_.x().coordinates = static_cast<Eigen::Vector3d>(coordinates.row(0));
    verticies_.y().coordinates = static_cast<Eigen::Vector3d>(coordinates.row(1));
    verticies_.z().coordinates = static_cast<Eigen::Vector3d>(coordinates.row(2));
}
Triangle::Vector3d Triangle::CalculateCoordinatesFromBarycentric(
    const Vector3d &coordinates) const {
    Vector3d ans = Vector3d::Zero();
    ans = verticies_.x().coordinates.GetCoordinates() * coordinates.x() +
          verticies_.y().coordinates.GetCoordinates() * coordinates.y() +
          verticies_.z().coordinates.GetCoordinates() * coordinates.z();
    return ans;
}
BarycentricCoordinateSystem::BarycentricCoordinateSystem(const Triangle &original,
                                                         const Triangle &transformed)
    : original_(original), transformed_(transformed) {
    for (int i = 0; i < 3; ++i) {
        original_coordinates_matrix_.row(i) =
            original_.GetVerticies()[i].coordinates.GetHomogeneousCoordinates();
        transformed_coordinates_matrix_.row(i) =
            transformed_.GetVerticies()[i].coordinates.GetHomogeneousCoordinates();
    }
}
Eigen::Vector3<BarycentricCoordinateSystem::BCoordinates>
BarycentricCoordinateSystem::GetTriangle() {
    return {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
}
BarycentricCoordinateSystem::Vector3d BarycentricCoordinateSystem::GetOriginalCoordinates(
    const BCoordinates &coordinates) const {
    return original_coordinates_matrix_.topLeftCorner<3, 3>() * coordinates;
}
BarycentricCoordinateSystem::Vector3d BarycentricCoordinateSystem::GetTransformedCoordinates(
    const BCoordinates &coordinates) const {
    return transformed_coordinates_matrix_.topLeftCorner<3, 3>() * coordinates;
}
Eigen::Vector3<Vertex> &Triangle::GetVerticies() {
    return verticies_;
}
const Eigen::Vector3<Vertex> &Triangle::GetVerticies() const {
    return verticies_;
}
Eigen::Vector3<Triangle::Vector3d> Triangle::GetVerticiesCoordinates() const {
    Eigen::Vector3<Triangle::Vector3d> ans;
    ans.x() = verticies_.x().coordinates.GetCoordinates();
    ans.y() = verticies_.y().coordinates.GetCoordinates();
    ans.z() = verticies_.z().coordinates.GetCoordinates();
    return ans;
}
Eigen::Vector3<Eigen::Vector3d> BarycentricCoordinateSystem::GetTransformedTriangleCoordinates(
    const Eigen::Vector3<BarycentricCoordinateSystem::BCoordinates> &triangle) const {
    Eigen::Vector3<Vector3d> ans;

    return ans;
}
Eigen::Vector3<Eigen::Vector3d> BarycentricCoordinateSystem::GetTriangleCoordinates(
    const Eigen::Vector3<BarycentricCoordinateSystem::BCoordinates> &triangle) const {
    Eigen::Vector3<Vector3d> ans;
    ans.x() = GetOriginalCoordinates(triangle.x());
    ans.y() = GetOriginalCoordinates(triangle.y());
    ans.z() = GetOriginalCoordinates(triangle.z());
    return ans;
}
double BarycentricCoordinateSystem::InterpolateZCoordinate(
    const BarycentricCoordinateSystem::BCoordinates &coordinates) {
    return transformed_coordinates_matrix_.col(3).dot(coordinates);
}
RGB BarycentricCoordinateSystem::GetColor(Vector3d b_coordinate) const {
    return {1, 0, 0};
}
}  // namespace renderer
