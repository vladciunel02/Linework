//
// Created by vlad on 10/27/26.
//
#include "Rectangle.h"
#include "Point.h"
#include "JsonHelper.h"
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
double Rectangle::perimeter()  const {
    return 2 * (getWidth() + getHeight());
}
double Rectangle::area() const {
    return getWidth() * getHeight();
}
std::unique_ptr<Rectangle> Rectangle::fromJson(const nlohmann::json &json) {
    return readBoxShape<Rectangle>(json);
}

