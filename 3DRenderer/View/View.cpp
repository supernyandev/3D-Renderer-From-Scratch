#include "View.h"
#include <cmath>
namespace view {
View::View(int width, int height)
    : recieve_port_([](std::shared_ptr<const renderer::Screen>) {},
                    [this](std::shared_ptr<const renderer::Screen> screen) { Draw(screen); },
                    [](std::shared_ptr<const renderer::Screen>) {}),
      window_(sf::VideoMode(width, height), "renderer") {
}

View::RGBA View::DRGBTo8BitRGBA(const renderer::RGB &color) {
    RGBA ans;
    ans.A = 255;
    ans.R = std::max(static_cast<uint8_t>(color.GetR() * 255.0), uint8_t{0});
    ans.G = std::max(static_cast<uint8_t>(color.GetG() * 255.0), uint8_t{0});
    ans.B = std::max(static_cast<uint8_t>(color.GetB() * 255.0), uint8_t{0});

    return ans;
}
void View::Draw(std::shared_ptr<const renderer::Screen> screen) {
    MatrixXRGBA matrix = Make8BitRgbMatrix(screen->GetScreenMatrix());
    sf::Image image;
    image.create(screen->GetWidth(), screen->GetHeight(),
                 reinterpret_cast<const uint8_t *>(matrix.data()));
    sf::Texture text;
    render_texture_.loadFromImage(image);
    sf::Sprite sprite;
    sprite.setTexture(render_texture_);
    window_.draw(sprite);
    window_.display();
}
View::MatrixXRGBA View::Make8BitRgbMatrix(const Eigen::MatrixX<renderer::RGB> &matrix) {
    MatrixXRGBA ans(matrix.rows(), matrix.cols());
    for (int i = 0; i < matrix.rows(); ++i) {
        for (int j = 0; j < matrix.cols(); ++j) {
            ans(i, j) = View::DRGBTo8BitRGBA(matrix(i, j));
        }
    }
    return ans;
}
bool View::GetEvent(sf::Event &event) {
    return window_.pollEvent(event);
}
View::RecievePortType *View::GetRecievePort() {
    return &recieve_port_;
}
}  // namespace view
