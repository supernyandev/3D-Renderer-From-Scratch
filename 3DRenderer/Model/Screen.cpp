#include "Screen.h"
renderer::Screen::Screen(size_t width, size_t height) : screen_matrix_(height, width) {
}
void renderer::Screen::SetPixel(size_t row, size_t column, const RGB &color) {
    screen_matrix_(row, column) = color;
}
const renderer::RGB &renderer::Screen::GetPixel(size_t row, size_t column) const {
    return screen_matrix_(row, column);
}
int renderer::Screen::GetWidth() const {
    return screen_matrix_.cols();
}

int renderer::Screen::GetHeight() const {
    return screen_matrix_.rows();
}
const Eigen::Matrix<renderer::RGB, Eigen::Dynamic, Eigen::Dynamic> &
renderer::Screen::GetScreenMatrix() const {
    return screen_matrix_;
}
