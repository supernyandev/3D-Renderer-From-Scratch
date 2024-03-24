#include "Application.h"
#include "Controller/Controller.h"
#include <iostream>
namespace application {
void HandleException() {

    try {
        throw;
    } catch (std::exception& e) {
        std::cerr << "Application failed with exception:" << e.what();

    } catch (...) {
        std::cerr << "Application failed with unknown exception";
    }
}
void Application::Run() {
    model::Model model(Application::GetWidth(), Application::GetHeight());
    view::View view(Application::GetWidth(), Application::GetHeight());
    controller::Controller controller;
    model.Subscribe(view.GetRecievePort());
    bool closed = false;
    while (true) {
        sf::Event event;
        while (view.GetEvent(event)) {
            closed = controller.HandleEvent(event, model);
        }
        if (closed) {
            break;
        }
    }
}

constexpr int Application::GetWidth() {
    return 1500;
}
constexpr int Application::GetHeight() {
    return 1000;
}
}  // namespace application
