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
void Shape::resize(double newWidth, double newHeight) {
    if (newWidth <= 0 || newHeight <= 0) {
        throw std::invalid_argument("Width and height must be positive.");
    }
    width = newWidth;
    height = newHeight;
}
void Shape::setFillColor(const Color& color){
    Style newStyle(color, style.getStrokeColor(), style.getStrokeWidth());
    style = newStyle;
}
void Shape::setStrokeColor(const Color& color){
    Style newStyle(style.getFillColor(), color, style.getStrokeWidth());
    style = newStyle;
}
void Shape::setStrokeWidth(double newWidth){
    Style newStyle(style.getFillColor(), style.getStrokeColor(), newWidth);
    style = newStyle;
}
void Shape::setStyle(const Color& fillColor, const Color& strokeColor, double strokeWidth){
    Style newStyle(fillColor, strokeColor, strokeWidth);
    style = newStyle;
}
void Shape::scale(double factor) {
    if (factor <= 0) {
        throw std::invalid_argument("Scale factor must be positive.");
    }
    width *= factor;
    height *= factor;
}
std::ostream& operator<<(std::ostream& os, const Shape& shape) {
    os << shape.getShapeType() << " at " << shape.position << " with width " << shape.width
       << ", height " << shape.height << ", style: " << shape.style;
    return os;
}
