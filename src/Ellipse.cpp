#include <numbers>
#include <cmath>
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
std::unique_ptr<Shape> Ellipse::clone() const{
    return std::make_unique<Ellipse>(*this);
}
double Ellipse::area() const {
    return  std::numbers::pi * (getWidth() / 2) * (getHeight() / 2);
}
double Ellipse::perimeter() const {
    // Aproximarea lui Ramanujan pentru perimetrul elipsei
    double a = getWidth() / 2;  // semi-major axis
    double b = getHeight() / 2; // semi-minor axis
    return std::numbers::pi * (3 * (a + b) - std::sqrt((3 * a + b) * (a + 3 * b)));
}