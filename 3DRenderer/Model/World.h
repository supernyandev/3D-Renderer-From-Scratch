#pragma once

#include <vector>
#include "AnyObject.h"
#include "Camera.h"
#include "Eigen/src/Geometry/AngleAxis.h"

namespace renderer {
class World {
public:
    using Vector3d = Eigen::Vector3d;
    using Vector4d = Eigen::Vector4d;
    using Quaterniond = Eigen::Quaterniond;
    World(int width, int height);
    auto GetObjectsIterable() const {
        return Iterable(objects_.begin(), objects_.end());
    };

    void AddObject(AnyObject);

    void AddObject(AnyObject, const Vector3d &);

    const Quaterniond &GetCameraRotation() const;

    const Vector3d &GetCameraPosition() const;

    const Camera &GetCamera() const;

    class CameraHolder : public Camera {
    public:
        CameraHolder(Camera);
        const Vector3d &GetCoordinates() const;
        const Quaterniond &GetAngle() const;

        void SetCoordinates(const Vector3d &);
        void SetAngle(const Quaterniond &);

    private:
        Eigen::Vector3d coordinates_ = World::GetOrigin();
        Eigen::Quaterniond rotation_;
    };

    class ObjectHolder : public AnyObject {
    public:
        ObjectHolder(AnyObject &&, const Vector3d &, const Eigen::AngleAxisd &);
        ObjectHolder(const AnyObject &);
        ObjectHolder(AnyObject &&) noexcept;
        const Vector3d &GetCoordinates() const;
        const Quaterniond &GetAngle() const;

        void SetCoordinates(const Vector3d &);
        void SetAngle(const Quaterniond &);
        void SetAngle(const Eigen::AngleAxis<double> &);

    private:
        Eigen::Vector3d coordinates_ = World::GetOrigin();
        Eigen::Quaterniond rotation_;
    };

    static Vector3d GetOrigin();

private:
    std::vector<ObjectHolder> objects_;
    std::vector<CameraHolder> cameras_;
    size_t current_camera_ind_ = 0;
};
}  // namespace renderer
