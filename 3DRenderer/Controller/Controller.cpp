#include "Controller.h"
#include <SFML/Window/Keyboard.hpp>
#include <memory>
namespace controller {
Controller::Controller() {
}

bool Controller::HandleEvent(const sf::Event& event, model::Model& model) {
    switch (event.type) {
        case sf::Event::Closed:
            return true;
        case sf::Event::KeyPressed:
            HandleKeyEvent(event, model);
            break;
        default:
            break;
    }
    return false;
}
bool Controller::HandleKeyEvent(const sf::Event& event, model::Model& model) {

    switch (event.key.code) {

        case sf::Keyboard::A:
            model.TestUpdateProjection();
            break;
        default:
            break;
    }
    return false;
}
}  // namespace controller
