//
// Created by vlad on 10/24/26.
//
#include "Point.h"
#include <cmath>

Point::Point(double x_pos, double y_pos) : x_pos{x_pos}, y_pos{y_pos} {}

Point Point::moveBy(double xOffset, double yOffset) const {
    return Point{x_pos + xOffset, y_pos + yOffset};
}

double Point::distanceTo(Point other) const {
    return std::sqrt(pow(x_pos - other.x_pos, 2) + pow(y_pos - other.y_pos, 2));
}

std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "(" << point.x_pos << ", " << point.y_pos << ")";
    return os;
}