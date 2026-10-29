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
bool Canvas::containsPoint (const Point& point) const {
    double x = point.getX();
    double y = point.getY();
    return (x >= 0 && x <= width && y >= 0 && y <= height);
}
std::ostream& operator<<(std::ostream& os, const Canvas& canvas) {
    os << "Canvas(width: " << canvas.width << ", height: " << canvas.height << ")";
    return os;
}
