//
// Created by vlad on 10/26/26.
//
#include "Shape.h"
#include <stdexcept>

Shape::Shape(const Style &style, const Point &position, double width, double height)
    : style{style}, position{position}, width{width}, height{height} {
        if (width <= 0 || height <= 0) {
            throw std::invalid_argument("Width and height must be positive.");
        }

    }
void Shape::moveTo(const Point &newPosition) {
    position = newPosition;
}
void Shape::moveBy(double xOffset, double yOffset) {
    position = position.moveBy(xOffset, yOffset);
}

std::ostream& operator<<(std::ostream& os, const Shape& shape) {
    os << shape.getShapeType() << " at " << shape.position << " with width " << shape.width
       << ", height " << shape.height << ", style: " << shape.style;
    return os;
}
