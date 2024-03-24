#include "Application.h"

int main() {
    try {
        application::Application application;
        application.Run();
    } catch (...) {
        application::HandleException();
    }
    return 0;
}
