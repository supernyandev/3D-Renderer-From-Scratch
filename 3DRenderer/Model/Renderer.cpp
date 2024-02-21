#include "Renderer.h"
#include "Primitives.h"
#include <cassert>
namespace renderer {
Screen Renderer::Draw(const World &world, size_t width, size_t height) {
    Screen screen(width, height);
    for (const auto &object : world.GetObjectsIterable()) {
        for (const auto &triangle : object->GetMesh().GetTriangles()) {
            DrawTriangle(triangle, object, world, screen);
        }
    }
    return screen;
}

void Renderer::ShiftTriangleCoordinates(const World::ObjectHolder &owner, Triangle *vertices) {
    assert(vertices != nullptr);
    Matrix4d transformation_matrix =
        Renderer::MakeHomogeneousTransformationMatrix(owner.GetAngle(), owner.GetCoordinates());
    Renderer::ApplyHomogeneousTransformationMatrix(transformation_matrix, &(*vertices));
}

void Renderer::ShiftTriangleToAlignCamera(const World &world, Triangle *vertices) {
    assert(vertices != nullptr);
    Matrix4d transformation_matrix = Renderer::MakeHomogeneousTransformationMatrix(
        world.GetCameraRotation().inverse(), -world.GetCameraPosition());

    Renderer::ApplyHomogeneousTransformationMatrix(transformation_matrix, vertices);
}

void Renderer::DrawTriangle(const Mesh::ITriangle &current, const World::ObjectHolder &owner_object,
                            const World &world, Screen &screen) {

    Triangle vertices = owner_object->GetMesh().GetTriangleVertices(current);

    ShiftTriangleCoordinates(owner_object, &vertices);

    ShiftTriangleToAlignCamera(world, &vertices);

    Triangle transformed_vertices = world.GetCamera().ApplyPerspectiveTransformation(vertices);

    BarycentricCoordinateSystem system(vertices, transformed_vertices);
    // TODO: clip triangle
    // for each clipped:
    // TODO: convert to matrix
    // RasterizeTriangle(system, system.GetTriangle(), &screen);
}
namespace {
using Vector3d = Eigen::Vector3d;
Eigen::Matrix3d TransformToScreenSpace(Eigen::Matrix3d triangle, size_t width, size_t height) {
    assert(width != 0);
    assert(height != 0);
    double dwidth = static_cast<double>(width);
    double dheight = static_cast<double>(height);

    triangle.col(0) += Eigen::Vector3d::Ones();
    triangle.col(0) *= dwidth / 2;

    triangle.col(1) = -triangle.col(1);
    triangle.col(1) += Eigen::Vector3d::Ones();
    triangle.col(1) *= dheight / 2;
    return triangle;
}
Eigen::Matrix3d TransformToCameraSpace(Eigen::Matrix3d triangle, size_t width, size_t height) {
    assert(width != 0);
    assert(height != 0);
    double dwidth = static_cast<double>(width);
    double dheight = static_cast<double>(height);

    triangle.col(0) /= dwidth / 2;
    triangle.col(0) -= Eigen::Vector3d::Ones();

    triangle.col(1) /= dheight / 2;
    triangle.col(1) -= Eigen::Vector3d::Ones();
    triangle.col(1) = -triangle.row(1);
    return triangle;
}
Vector3d ConvertToBarycentric(Eigen::Vector2d coordinates, const Eigen::Matrix3d &triangle) {
    Vector3d ans;
    coordinates -= triangle.row(2).topLeftCorner<1, 2>();
    Eigen::Matrix2d temp;
    temp = triangle.topLeftCorner<2, 2>().transpose();
    temp.col(0) -= triangle.row(2).topLeftCorner<1, 2>();
    temp.col(1) -= triangle.row(2).topLeftCorner<1, 2>();
    ans.topLeftCorner<2, 1>() = temp.inverse() * coordinates;
    ans.z() = 1 - ans.x() - ans.y();
    return ans;
}
bool CheckIfInside(const Vector3d &b_coordinate) {
    return b_coordinate.x() <= 1.0 && b_coordinate.x() >= 0.0 && b_coordinate.y() <= 1.0 &&
           b_coordinate.y() >= 0.0 && b_coordinate.z() <= 1.0 && b_coordinate.z() >= 0;
}
};  // namespace
void Renderer::RasteriseTriangle(const BarycentricCoordinateSystem &system,
                                 const Eigen::Matrix3d &coordinates, Screen *screen) {
    size_t height = screen->GetHeight();
    size_t width = screen->GetWidth();
    Eigen::Matrix3d screen_triangle = TransformToScreenSpace(coordinates, width, height);
    size_t min_x = -1;
    size_t min_y = -1;
    size_t max_x = 0;
    size_t max_y = 0;
    for (int i = 0; i < 3; ++i) {
        min_x = std::min(static_cast<size_t>(std::floor(screen_triangle.row(i).x())), min_x);
        min_y = std::min(static_cast<size_t>(std::floor(screen_triangle.row(i).y())), min_y);
        max_x = std::max(static_cast<size_t>(std::ceil(screen_triangle.row(i).x())), max_x);
        max_y = std::max(static_cast<size_t>(std::ceil(screen_triangle.row(i).y())), max_y);
    }
    for (size_t x = min_x; x <= max_x; ++x) {
        for (size_t y = min_y; y <= max_y; ++y) {
            Eigen::Vector2d vec = {static_cast<double>(x) + 0.5, static_cast<double>(y) + 0.5};
            Vector3d b_coordinate = ConvertToBarycentric(vec, screen_triangle);
            if (CheckIfInside(b_coordinate)) {
                // TODO: z-buffer
                screen->SetPixel(y, x, system.GetColor(b_coordinate));
            }
        }
    }
}
Renderer::Matrix4d Renderer::MakeHomogeneousTransformationMatrix(const Quaterniond &rotation,
                                                                 const Vector3d &offset) {

    Matrix4d transformation_matrix = Matrix4d::Zero();
    transformation_matrix.topLeftCorner<3, 3>() = rotation.toRotationMatrix();
    transformation_matrix(3, 3) = 1;
    transformation_matrix.col(3).topLeftCorner<3, 1>() = offset;
    return transformation_matrix;
}
void Renderer::ApplyHomogeneousTransformationMatrix(const Eigen::Matrix4d &transformation_matrix,
                                                    Triangle *vertices) {
    assert(vertices != nullptr);

    // Page 76 "Mathematics for 3D game..."
    for (auto &ver : vertices->GetVerticies()) {
        ver.coordinates = transformation_matrix * ver.coordinates.GetHomogeneousCoordinates();
        ver.normal = transformation_matrix * ver.normal.GetHomogeneousCoordinates();
    }
}
}  // namespace renderer
