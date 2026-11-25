//
// Created by vlad on 10/28/26.
//
#undef NDEBUG
#include "canvas_test.h"
#include "Canvas.h"
#include <cassert>
#include <stdexcept>

void testCanvasConstructor() {
    const Canvas canvas(800, 600);
    assert(canvas.getWidth() == 800 && canvas.getHeight() == 600 && "the canvas keeps the size it was built with");

    bool threw = false;
    try {
        const Canvas zero(0, 600);
        (void) zero;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "a canvas with zero width is rejected");

    threw = false;
    try {
        const Canvas tooBig(Canvas::maxWidth + 1, 600);
        (void) tooBig;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "a canvas larger than the maximum is rejected");
}

void testCanvasResize() {
    Canvas canvas(800, 600);
    canvas.resize(1024, 768);
    assert(canvas.getWidth() == 1024 && canvas.getHeight() == 768 && "resize changes both dimensions");

    bool threw = false;
    try {
        canvas.resize(1024, -1);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "a negative height is rejected");
    assert(canvas.getWidth() == 1024 && canvas.getHeight() == 768 && "a failed resize leaves the canvas unchanged");
}