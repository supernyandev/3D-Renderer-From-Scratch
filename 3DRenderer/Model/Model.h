#pragma once
#include "Renderer.h"
#include "../Observer/Observer.h"
#include "Screen.h"
#include "Utils.h"
namespace model {
class Model {
public:
    Model(int width, int height);
    void Subscribe(observer::CObserver<std::shared_ptr<const renderer::Screen>> *obs);
    void TestUpdateRasterization();
    void TestUpdateProjection();

private:
    std::optional<std::shared_ptr<const renderer::Screen>> GetCurrentScreen();
    renderer::Renderer renderer_;
    renderer::World world_;
    observer::CObservable<std::shared_ptr<const renderer::Screen>> update_port_;
    std::shared_ptr<const renderer::Screen> current_screen_;
    int width_;
    int height_;

    std::vector<renderer::RGB> test_colors_ = {{0.5, 0.5, 0.5}, {0, 0, 0}};
    int test_current_index_ = 0;
    double test_current_plane_d_ = 1;
};

};  // namespace model
