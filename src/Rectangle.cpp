//
// Created by vlad on 10/27/26.
//
#include "Rectangle.h"
#include "Point.h"
Rectangle::Rectangle(const Style &style, const Point &position, double width, double height)
    : Shape(style, position, width, height) {}

bool Rectangle::containsPoint(const Point &point) const {
    double x = point.getX();
    double y = point.getY();
    double rectX = getPosition().getX();
    double rectY = getPosition().getY();
    double rectWidth = getWidth();
    double rectHeight = getHeight();
    if(x >= rectX && x <= rectX + rectWidth &&
       y >= rectY && y <= rectY + rectHeight) {
        return true;
    }
    return false;
}

std::unique_ptr<Shape> Rectangle::clone() const {
    return std::make_unique<Rectangle>(*this);
}
