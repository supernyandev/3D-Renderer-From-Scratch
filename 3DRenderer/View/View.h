#pragma once
#include "../Observer/Observer.h"
#include <memory>
#include "../Model/Screen.h"
#include <SFML/Graphics.hpp>
namespace view {
class View {
public:
    using RecievePortType = observer::CObserver<std::shared_ptr<const renderer::Screen>>;
    struct RGBA {
        uint8_t R, G, B, A;
    };
    using MatrixXRGBA = Eigen::Matrix<RGBA, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
    View(int width, int height);
    bool GetEvent(sf::Event &event);
    RecievePortType *GetRecievePort();

private:
    static RGBA DRGBTo8BitRGBA(const renderer::RGB &color);
    void Draw(std::shared_ptr<const renderer::Screen>);
    MatrixXRGBA Make8BitRgbMatrix(const Eigen::MatrixX<renderer::RGB> &matrix);
    RecievePortType recieve_port_;
    sf::RenderWindow window_;
    sf::Texture render_texture_;
};
}  // namespace view
