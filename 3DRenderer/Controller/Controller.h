#pragma once
#include "../Observer/Observer.h"
#include "../Model/Screen.h"
#include "../Model/Model.h"
#include <SFML/Graphics.hpp>
#include <memory>
namespace controller {
class Controller {
public:
    using RecievePortType = observer::CObserver<std::shared_ptr<const renderer::Screen>>;
    Controller();
    RecievePortType *GetRecievePort();
    bool HandleEvent(const sf::Event &, model::Model &);
    bool HandleKeyEvent(const sf::Event &, model::Model &);

private:
};

}  // namespace controller
