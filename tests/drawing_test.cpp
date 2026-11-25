#undef NDEBUG
#include <cassert>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "drawing_test.h"
#include "Drawing.h"

// toate testele de incarcare folosesc fisierul din assets/


void testDrawingLoadFromFile() {
    const Drawing drawing = Drawing::loadFromFile("assets/drawing.json");

    std::ostringstream out;
    out << drawing;
    const std::string text = out.str();

    assert(text.find("Drawing: Primul meu desen") == 0 && "the drawing prints its name first");
    assert(text.find("Fundal") != std::string::npos && "the first layer was loaded");
    assert(text.find("Detalii") != std::string::npos && "the second layer was loaded");
    assert(text.find("Rectangle") != std::string::npos && "rectangles were loaded");
    assert(text.find("Ellipse") != std::string::npos && "ellipses were loaded");
}

void testDrawingShapeUnderCursor() {
    Drawing drawing = Drawing::loadFromFile("assets/drawing.json");
    Layer& fundal = drawing.getLayer("Fundal");
    Layer& detalii = drawing.getLayer("Detalii");

    // (300, 200): dreptunghiul rosu si elipsa galbena se suprapun pe Fundal
    const Shape* onOverlap = drawing.getShapeUnderCursor(Point{300, 200});
    const Shape* fromFundal = fundal.getShapeUnderCursor(Point{300, 200});
    assert(onOverlap != nullptr && onOverlap == fromFundal && "the drawing finds the top shape of the layer");

    // (580, 150): elipsa albastra de pe Detalii
    const Shape* onDetalii = drawing.getShapeUnderCursor(Point{580, 150});
    const Shape* fromDetalii = detalii.getShapeUnderCursor(Point{580, 150});
    assert(onDetalii != nullptr && onDetalii == fromDetalii && "the drawing also finds shapes on the second layer");

    // (850, 500): dreptunghiul verde iese din panza, dar formele pot sta si in afara ei
    const Shape* outsideFromLayer = detalii.getShapeUnderCursor(Point{850, 500});
    const Shape* outsideFromDrawing = drawing.getShapeUnderCursor(Point{850, 500});
    assert(outsideFromLayer != nullptr && "the green rectangle really covers that point");
    assert(outsideFromDrawing == outsideFromLayer && "a shape can be selected where it sticks out of the canvas");

    // (2000, 2000): departe de panza si de orice forma
    const Shape* farAway = drawing.getShapeUnderCursor(Point{2000, 2000});
    assert(farAway == nullptr && "a click far from every shape selects nothing");

    // (600, 400): panza goala
    const Shape* empty = drawing.getShapeUnderCursor(Point{600, 400});
    assert(empty == nullptr && "a click on empty canvas selects nothing");
}

void testDrawingLayers() {
    Drawing drawing{"Test", Canvas{100, 100}};
    drawing.addLayer(Layer{"A"});

    const Layer& found = drawing.getLayer("A");
    assert(found.getName() == "A" && "getLayer finds a layer by name");

    bool threw = false;
    try {
        drawing.addLayer(Layer{"A"});
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "two layers cannot have the same name");

    drawing.removeLayer("A");
    threw = false;
    try {
        const Layer& removed = drawing.getLayer("A");
        (void) removed;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "a removed layer cannot be found anymore");

    threw = false;
    try {
        drawing.removeLayer("X");
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "removing a missing layer is rejected");
}

void testDrawingInvalid() {
    bool threw = false;
    try {
        const Drawing bad{"   ", Canvas{100, 100}};
        (void) bad;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "a drawing needs a non-blank name");

    threw = false;
    try {
        const Drawing missing = Drawing::loadFromFile("assets/nu_exista.json");
        (void) missing;
    } catch (const std::runtime_error&) {
        threw = true;
    }
    assert(threw && "a missing file is reported with runtime_error");

    // un tip de forma pe care ShapeFactory nu il cunoaste
    const std::string unknownTypeFile = "unknown_type_test.json";
    {
        std::ofstream file(unknownTypeFile);
        file << R"({"name": "Test", "canvas": {"width": 100, "height": 100},
                   "layers": [{"name": "A", "shapes": [{"type": "Triangle"}]}]})";
    }
    threw = false;
    try {
        const Drawing unknown = Drawing::loadFromFile(unknownTypeFile);
        (void) unknown;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    std::remove(unknownTypeFile.c_str());
    assert(threw && "an unknown shape type in the file is rejected");
}

void testDrawingResizeCanvas() {
    Drawing drawing = Drawing::loadFromFile("assets/drawing.json");

    // panza se poate micsora chiar daca unele forme raman in afara ei
    drawing.resizeCanvas(400, 300);
    std::ostringstream out;
    out << drawing;
    assert(out.str().find("Canvas(width: 400, height: 300)") != std::string::npos && "the canvas shrinks even with shapes outside it");

    // formele raman neatinse si pot fi selectate in continuare
    const Shape* green = drawing.getShapeUnderCursor(Point{850, 500});
    assert(green != nullptr && "shapes left outside the canvas are still there");

    // dimensiunile invalide sunt refuzate in continuare
    bool threw = false;
    try {
        drawing.resizeCanvas(0, 100);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "the canvas needs positive dimensions");

    threw = false;
    try {
        drawing.resizeCanvas(Canvas::maxWidth + 1, 100);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "the canvas cannot be larger than the maximum size");
}