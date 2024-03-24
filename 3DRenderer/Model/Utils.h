#pragma once
#include "Eigen/Dense"
#include "Eigen/src/Core/Matrix.h"
#include <initializer_list>
#include <list>
namespace renderer {
class RGB {
public:
    using Vector3d = Eigen::Vector3d;
    RGB(const std::initializer_list<double> &);
    RGB();
    double GetR() const;
    void SetR(double);
    double GetG() const;
    void SetG(double);
    double GetB() const;
    void SetB(double);

private:
    // r,g,b
    Vector3d val_;
};
template <typename Iterator>
class Iterable {
public:
    Iterable(Iterator begin, Iterator end) : begin_(begin), end_(end) {
    }
    Iterator begin() {  // NOLINT
        return begin_;
    }
    Iterator end() {  // NOLINT
        return end_;
    }

private:
    Iterator begin_;
    Iterator end_;
};

class Direction {
public:
    using Vector3d = Eigen::Vector3d;
    using Vector4d = Eigen::Vector4d;

    Direction() = default;
    Direction(const Vector3d &);
    Direction(const Vector4d &);
    template <typename T>
    Direction(const T &evaluable) {
        *this = evaluable.eval();
    }
    Vector3d GetCoordinates() const;
    Vector4d GetHomogeneousCoordinates() const;
    template <typename T>
    Direction &operator=(const T &evaluable) {
        return *this = evaluable.eval();
    }
    Direction &operator=(const Vector3d &);
    Direction &operator=(const Vector4d &);

private:
    Vector3d data_ = Vector3d::Zero();
};

class Point {
public:
    using Vector3d = Eigen::Vector3d;
    using Vector4d = Eigen::Vector4d;
    Point() = default;
    Point(const Vector3d &);
    Point(const Vector4d &);
    template <typename T>
    Point(const T &evaluable) {
        *this = evaluable.eval();
    }
    Vector3d GetCoordinates() const;
    const Vector4d &GetHomogeneousCoordinates() const;
    template <typename T>
    Point &operator=(const T &evaluable) {
        return *this = (evaluable.eval());
    }
    double w();  // NOLINT
    Point &operator=(const Vector3d &);
    Point &operator=(const Vector4d &);

private:
    Vector4d data_ = Vector4d::Ones();
};

bool DetermineSide(const Eigen::Vector4d &plane, const Eigen::Vector3d &point);
Eigen::Vector3d PlaneLineIntersection(const Eigen::Vector4d &plane, const Eigen::Vector3d &point1,
                                      const Eigen::Vector3d &point2);
// true if clipped
bool ClipOneTriangle(const Eigen::Vector4d &plane, const Eigen::Matrix3d &triangle,
                     std::list<Eigen::Matrix3d> *triangles);
void ClipAllTriangles(const Eigen::Vector4d &plane, std::list<Eigen::Matrix3d> *triangles);
}  // namespace renderer
