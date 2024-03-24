#pragma once

#include <Eigen/Dense>

#include "Utils.h"
#include "Vertex.h"
namespace renderer {
class Triangle {
public:
    using Vector3d = Eigen::Vector3d;
    Triangle() = default;
    Triangle(Eigen::Matrix3d);
    Vector3d CalculateCoordinatesFromBarycentric(const Vector3d &) const;
    Eigen::Vector3<Vertex> &GetVerticies();
    const Eigen::Vector3<Vertex> &GetVerticies() const;
    // bad design :-)
    Eigen::Vector3<Vector3d> GetVerticiesCoordinates() const;
    Eigen::Matrix<double, 3, 4> GetVerticesHomogeniousCoordinates() const;

private:
    Eigen::Vector3<Vertex> verticies_;
};
class BarycentricCoordinateSystem {
public:
    using Vector3d = Eigen::Vector3d;
    using BCoordinates = Vector3d;
    using Matrix34d = Eigen::Matrix<double, 3, 4>;
    using Matrix2d = Eigen::Matrix2d;
    Eigen::Vector3<BCoordinates> GetTriangle();
    BarycentricCoordinateSystem(const Triangle &original, const Matrix34d &transformed);
    Vector3d GetOriginalCoordinates(const BCoordinates &) const;
    Vector3d GetTransformedCoordinates(const BCoordinates &) const;
    Eigen::Vector3<Vector3d> GetTransformedTriangleCoordinates(
        const Eigen::Vector3<BCoordinates> &) const;
    Eigen::Vector3<Vector3d> GetTriangleCoordinates(const Eigen::Vector3<BCoordinates> &) const;
    double InterpolateZCoordinate(const BCoordinates &);
    RGB GetColor(Vector3d b_coordinate) const;
    Vector3d ConvertToBarycentricCoordinates(Eigen::Vector2d) const;

private:
    Triangle original_;
    Matrix34d original_coordinates_matrix_;
    Matrix34d transformed_coordinates_matrix_;
    Matrix2d barycentric_transformation_matrix_;
};
}  // namespace renderer
