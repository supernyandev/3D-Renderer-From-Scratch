#include "Utils.h"
#include <cassert>
namespace renderer {
RGB::RGB(const std::initializer_list<double>& init) {
    size_t i = 0;
    for (double num : init) {
        assert(i != 3);
        val_[i] = num;
        ++i;
    }
}
double renderer::RGB::GetR() const {
    return val_(0);
}
void renderer::RGB::SetR(double val) {
    val_(0) = val;
}
double renderer::RGB::GetG() const {
    return val_(1);
}
void renderer::RGB::SetG(double val) {
    val_(1) = val;
}
double renderer::RGB::GetB() const {
    return val_(2);
}
void renderer::RGB::SetB(double val) {
    val_(2) = val;
}
RGB::RGB() : val_(Vector3d::Zero()) {
}
Direction::Direction(const Vector3d& vec) : data_(vec) {
}
Direction::Direction(const Vector4d& vec) : data_(vec.topLeftCorner<3, 1>()) {
}
Direction& Direction::operator=(const Vector3d& coordinates) {
    data_ = coordinates;
    return *this;
}
Direction& Direction::operator=(const Vector4d& h_coordinates) {
    data_ = h_coordinates.topLeftCorner<3, 1>();
    return *this;
}
Direction::Vector3d Direction::GetCoordinates() const {
    return data_;
}
Direction::Vector4d Direction::GetHomogeneousCoordinates() const {
    Vector4d ans;
    ans.topLeftCorner<3, 1>() = data_;
    ans.w() = 0;
    return ans;
}
Point::Point(const Vector4d& vec) : data_(vec) {
}
Point::Point(const Vector3d& vec) {
    data_.topLeftCorner<3, 1>() = vec;
}
Point& Point::operator=(const Vector4d& h_coordinates) {
    data_ = h_coordinates;
    return *this;
}
Point& Point::operator=(const Vector3d& coordinates) {
    data_.topLeftCorner<3, 1>() = coordinates;
    data_.w() = 1;
    return *this;
}
Point::Vector3d renderer::Point::GetCoordinates() const {
    return data_.topLeftCorner<3, 1>();
}
const Point::Vector4d& Point::GetHomogeneousCoordinates() const {
    return data_;
}
double Point::w() {
    return data_.w();
}
bool DetermineSide(const Eigen::Vector4d& plane, const Eigen::Vector3d& point) {
    assert("Plane is not zero" && (!(plane.x() == 0 && plane.y() == 0 && plane.z() == 0)));
    Eigen::Vector3d point_on_plane = plane.topLeftCorner<3, 1>() * (-plane.w());
    Eigen::Vector3d point_vector = point - point_on_plane;
    double scalar_prod = plane.topLeftCorner<3, 1>().dot(point_vector);
    return scalar_prod > 0;
}
Eigen::Vector3d PlaneLineIntersection(const Eigen::Vector4d& plane, const Eigen::Vector3d& point1,
                                      const Eigen::Vector3d& point2) {
    // page 98
    Eigen::Vector3d ans;
    Eigen::Vector3d v = point2 - point1;
    double t = (-(plane.topLeftCorner<3, 1>().dot(point1) + plane.w())) /
               (plane.topLeftCorner<3, 1>().dot(v));
    ans = point1 + t * v;
    return ans;
}
bool ClipOneTriangle(const Eigen::Vector4d& plane, const Eigen::Matrix3d& triangle,
                     std::list<Eigen::Matrix3d>* triangles) {
    int cnt = 0;

    Eigen::Matrix3d points_inside;
    Eigen::Matrix3d points_outside;
    for (int i = 0; i < 3; ++i) {
        bool is_inside = DetermineSide(plane, triangle.row(i));
        if (is_inside) {
            ++cnt;
            points_inside.row(cnt - 1) = triangle.row(i);
        } else {
            points_outside.row(i - cnt) = triangle.row(i);
        }
    }
    if (cnt == 3) {
        return false;
    }
    if (cnt == 0) {
        return true;
    }
    if (cnt == 1) {
        Eigen::Vector3d first_inter =
            PlaneLineIntersection(plane, points_outside.row(0), points_inside.row(0));
        Eigen::Vector3d second_inter =
            PlaneLineIntersection(plane, points_outside.row(1), points_inside.row(0));
        Eigen::Matrix3d ans;
        ans.row(0) = first_inter;
        ans.row(1) = second_inter;
        ans.row(2) = points_inside.row(0);
        triangles->push_back(ans);
    }
    if (cnt == 2) {

        Eigen::Vector3d first_inter =
            PlaneLineIntersection(plane, points_outside.row(0), points_inside.row(0));
        Eigen::Vector3d second_inter =
            PlaneLineIntersection(plane, points_outside.row(0), points_inside.row(1));
        Eigen::Matrix3d ans1;
        Eigen::Matrix3d ans2;
        ans1.row(0) = first_inter;
        ans1.row(1) = points_inside.row(0);
        ans1.row(2) = points_inside.row(1);

        ans2.row(0) = first_inter;
        ans2.row(1) = second_inter;
        ans2.row(2) = points_inside.row(1);

        triangles->push_back(ans1);
        triangles->push_back(ans2);
    }

    return true;
}
void ClipAllTriangles(const Eigen::Vector4d& plane, std::list<Eigen::Matrix3d>* triangles) {
    if (triangles->empty()) {
        return;
    }
    auto it = triangles->end();
    --it;
    while (true) {
        bool is_clipped = ClipOneTriangle(plane, *it, triangles);
        if (is_clipped) {
            bool is_begining = it == triangles->begin();
            auto new_it = triangles->erase(it);
            if (new_it == triangles->begin()) {
                break;
            }
            it = new_it;
            --it;
            continue;
        }
        if (it == triangles->begin()) {
            break;
        }
        --it;
    }
}
}  // namespace renderer
