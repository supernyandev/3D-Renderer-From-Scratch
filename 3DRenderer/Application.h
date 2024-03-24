#pragma once
#include "Model/Model.h"
#include "Controller/Controller.h"
#include "View/View.h"

namespace application {
void HandleException();
class Application {
public:
    void Run();

private:
    static constexpr int GetWidth();
    static constexpr int GetHeight();
};
}  // namespace application
