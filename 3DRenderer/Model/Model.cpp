#include "Model.h"
#include "Eigen/src/Core/Matrix.h"
#include "Primitives.h"
#include "Screen.h"
#include "Utils.h"
#include <algorithm>
#include <cmath>
#include <memory>
namespace model {
Model::Model(int width, int height)
    : width_(width), height_(height), update_port_([this]() { return GetCurrentScreen(); }) {
}

std::optional<std::shared_ptr<const renderer::Screen>> Model::GetCurrentScreen() {
    return current_screen_;
}

void Model::Subscribe(observer::CObserver<std::shared_ptr<const renderer::Screen>> *obs) {
    update_port_.subscribe(obs);
}
void Model::TestUpdate() {
    std::unique_ptr<renderer::Screen> screen = std::make_unique<renderer::Screen>(width_, height_);
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            screen->SetPixel(i, j, test_colors_[test_current_index_]);
            // plane cutting  check
            Eigen::Vector3d point = {
                (static_cast<double>(j) / static_cast<double>(width_)) * 2 - 1.0,
                (1 - static_cast<double>(i) / static_cast<double>(height_)) * 2.0, 0};
            Eigen::Vector4d plane = {1 / std::sqrt(2), 1 / std::sqrt(2), 0, 0};
            bool side = renderer::DetermineSide(plane, point);
            if (side) {
                screen->SetPixel(i, j, {0, 0.3, 0.6});
            }
        }
    }
    Eigen::Matrix3d tr;
    tr << -0.5, 0.5, 0, -0.5, 0, 0, 0.5, 0, 0;
    renderer::Triangle triangle(tr);
    renderer::BarycentricCoordinateSystem syst(triangle, triangle);
    renderer_.RasteriseTriangle(syst, tr, &(*screen));
    ++test_current_index_;
    test_current_index_ = test_current_index_ % 2;
    current_screen_.reset(screen.release());
    update_port_.notify();
}
}  // namespace model
