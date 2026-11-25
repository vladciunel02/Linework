//
// Created by vlad on 10/27/26.
//
#include "Canvas.h"
#include <stdexcept>

Canvas::Canvas(int width, int height) : width(width), height(height) {
    if (width <= 0 || height <= 0 || width > maxWidth || height > maxHeight) {
        throw std::invalid_argument("Invalid canvas dimensions");
    }
}

std::ostream& operator<<(std::ostream& os, const Canvas& canvas) {
    os << "Canvas(width: " << canvas.width << ", height: " << canvas.height << ")";
    return os;
}
void Canvas::resize(int newWidth, int newHeight) {
    if (newWidth <= 0 || newHeight <= 0 || newWidth > maxWidth || newHeight > maxHeight) {
        throw std::invalid_argument("Invalid canvas dimensions");
    }
    width = newWidth;
    height = newHeight;
}
