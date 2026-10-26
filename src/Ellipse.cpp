#include "Ellipse.h"
#include "Point.h"

Ellipse::Ellipse(const Style &style, const Point &position, double width, double height)
    : Shape(style, position, width, height) {}


bool Ellipse::containsPoint(const Point &point) const {
    double x = point.getX();
    double y = point.getY();
    double centerX = getPosition().getX() + getWidth() / 2;
    double centerY = getPosition().getY() + getHeight() / 2;
    double radiusX = getWidth() / 2;
    double radiusY = getHeight() / 2;
    return ((x - centerX) * (x - centerX)) / (radiusX * radiusX) +
           ((y - centerY) * (y - centerY)) / (radiusY * radiusY) <= 1;
}