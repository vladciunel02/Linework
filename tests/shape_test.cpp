#undef NDEBUG
#include <cassert>
#include <sstream>
#include <stdexcept>

#include "shape_test.h"
#include "Rectangle.h"
#include "Ellipse.h"

namespace {
    // stilul nu contează pentru testele astea, doar trebuie să existe unul valid
    Style makeStyle() {
        return Style{Color{"#FF0000"}, Color{"#000000"}, 1.0};
    }
}

void testRectangleContains() {
    const Rectangle r{makeStyle(), Point{100, 50}, 200, 80};
    assert(r.containsPoint(Point{150, 90}) && "point inside rectangle");
    assert(!r.containsPoint(Point{350, 90}) && "point right of rectangle");
    assert(r.containsPoint(Point{300, 130}) && "bottom-right corner counts as inside");
}

void testEllipseContains() {
    const Ellipse e{makeStyle(), Point{0, 0}, 100, 100};
    assert(e.containsPoint(Point{50, 50}) && "center inside ellipse");
    assert(e.containsPoint(Point{100, 50}) && "point on the edge inside ellipse");
    assert(!e.containsPoint(Point{2, 2}) && "bounding box corner outside ellipse");
}

void testShapeMove() {
    Rectangle r{makeStyle(), Point{0, 0}, 10, 10};

    r.moveBy(100, 0);
    assert(!r.containsPoint(Point{5, 5}) && "old area is empty after moveBy");
    assert(r.containsPoint(Point{105, 5}) && "shape is at the new place after moveBy");

    r.moveTo(Point{-50, -50});
    assert(r.containsPoint(Point{-45, -45}) && "moveTo puts the shape at the given position");
}

void testShapePrint() {
    const Rectangle r{makeStyle(), Point{0, 0}, 10, 10};
    const Ellipse e{makeStyle(), Point{0, 0}, 10, 10};

    // prin referință la Shape, ca să verificăm că apelul virtual alege tipul real
    const Shape& s1 = r;
    const Shape& s2 = e;

    std::ostringstream out1;
    out1 << s1;
    assert(out1.str().find("Rectangle") == 0 && "Rectangle prints its own type");

    std::ostringstream out2;
    out2 << s2;
    assert(out2.str().find("Ellipse") == 0 && "Ellipse prints its own type");
}

void testShapeInvalidSize() {
    bool threw = false;
    try {
        const Rectangle r{makeStyle(), Point{0, 0}, 0, 10};
        (void) r;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "zero width must be rejected");

    threw = false;
    try {
        const Ellipse e{makeStyle(), Point{0, 0}, 10, -5};
        (void) e;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "negative height must be rejected");
}