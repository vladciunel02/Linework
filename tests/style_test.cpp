//
// Created by vlad on 10/26/26.
//

#include "style_test.h"
#include "Style.h"
#include <cassert>
#include <stdexcept>

void testStyleConstructor() {
    Color fillColor("#FF0000");
    Color strokeColor("#00FF00");
    double strokeWidth = 2.5;

    Style style(fillColor, strokeColor, strokeWidth);

    
    assert(style.getFillColor() == fillColor);
    assert(style.getStrokeColor() == strokeColor);
    assert(style.getStrokeWidth() == strokeWidth);
}

void testStyleBadColor() {
    bool threw = false;
    try {
        Color fillColor("#FF0000");
        Color strokeColor("00FF00"); // Invalid color format (missing '#')
        double strokeWidth = 2.5;

        Style style(fillColor, strokeColor, strokeWidth);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "Style constructor must throw for invalid color format");
}

void testStyleBadStrokeWidth() {
    bool threw = false;
    try {
        Color fillColor("#FF0000");
        Color strokeColor("#00FF00");
        double strokeWidth = -2.5; // Invalid stroke width (negative)

        Style style(fillColor, strokeColor, strokeWidth);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "Style constructor must throw for invalid stroke width");
}