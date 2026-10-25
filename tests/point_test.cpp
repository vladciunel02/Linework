//
// Created by vlad on 10/25/26.
//
#undef NDEBUG
#include <cassert>

#include "point_test.h"
#include "Point.h"
#include "../include/Point.h"

void testPointDistance() {
    Point p1(0,0);
    Point p2(3,4);
    double dist = p1.distanceTo(p2);
    assert(dist == 5 && "Distance is not correct");
}

void testPointMove() {
    Point p1(0,0);
    Point p2 = p1.moveBy(3,3);
    assert(p2.getX() == 3 && p2.getY() == 3 && "Point is not moved correctly");
    assert(p1.getX() == 0 && p1.getY() == 0 && "The original point is not unchanged");
}
