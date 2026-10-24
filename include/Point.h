//
// Created by vlad on 10/24/26.
//

#ifndef PROIECTPOO_POINT_H
#define PROIECTPOO_POINT_H
#include <iostream>

class Point {
private:
    double x_pos;
    double y_pos;
public:
    double getX() const{return x_pos;}
    double getY() const{return y_pos;}

    Point(double x_pos, double y_pos);

    Point moveBy(double xOffset, double yOffset) const;

    double distanceTo(Point other) const;
    friend std::ostream& operator<<(std::ostream& os, const Point& point);
};

#endif
