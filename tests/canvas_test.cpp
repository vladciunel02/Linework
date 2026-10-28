//
// Created by vlad on 10/28/26.
//

#include "canvas_test.h"
#include "Canvas.h"
#include "Point.h"
#include <cassert>
void testConstructorValid() {
    Canvas canvas(800, 600);
    assert(canvas.containsPoint(Point(400, 300))); // Point inside the canvas
}

void testCanvasContainsPointInside() {
    Canvas canvas(800, 600);
    Point pointInside(400, 300);
    assert(canvas.containsPoint(pointInside)); // Point inside the canvas
}

void testCanvasContainsPointOutside() {
    Canvas canvas(800, 600);
    Point pointOutside(900, 700);
    assert(!canvas.containsPoint(pointOutside)); // Point outside the canvas
}