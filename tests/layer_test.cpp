#undef NDEBUG
#include <cassert>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "layer_test.h"
#include "Layer.h"
#include "Rectangle.h"
#include "Ellipse.h"

namespace {
    Style makeStyle() {
        return Style{Color{"#FF0000"}, Color{"#000000"}, 1.0};
    }
    //doua forme suprapuse
    Layer makeOverlappingLayer() {
        Layer layer{"Fundal"};
        layer.addShape(std::make_unique<Rectangle>(makeStyle(), Point{0, 0}, 100, 100));
        layer.addShape(std::make_unique<Rectangle>(makeStyle(), Point{40, 40}, 20, 20));
        return layer;
    }
}

void testLayerTopmostShape() {
    Layer layer = makeOverlappingLayer();

    Shape* big = layer.getShapeUnderCursor(Point{10, 10});    // doar dreptunghiul mare e aici
    Shape* top = layer.getShapeUnderCursor(Point{50, 50});    // aici se suprapun amandoua

    assert(big != nullptr && "click on the big rectangle finds it");
    assert(top != nullptr && top != big && "click on the overlap finds the shape on top");
    assert(layer.getShapeUnderCursor(Point{500, 500}) == nullptr && "click on empty canvas finds nothing");
}

void testLayerReorder() {
    Layer layer = makeOverlappingLayer();
    Shape* big = layer.getShapeUnderCursor(Point{10, 10});
    Shape* small = layer.getShapeUnderCursor(Point{50, 50});

    layer.bringToFront(big);
    assert(layer.getShapeUnderCursor(Point{50, 50}) == big && "bringToFront puts the big rectangle on top");

    layer.sendToBack(big);
    assert(layer.getShapeUnderCursor(Point{50, 50}) == small && "sendToBack puts it back under the small one");

    // o forma de pe alt layer nu poate fi mutata în ordinea layer-ului asta
    Layer other = makeOverlappingLayer();
    Shape* foreign = other.getShapeUnderCursor(Point{10, 10});
    bool threw = false;
    try {
        layer.bringToFront(foreign);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "bringToFront rejects a shape from another layer");
}

void testLayerCopyIsDeep() {
    Layer original = makeOverlappingLayer();
    Layer copy{original};

    Shape* inOriginal = original.getShapeUnderCursor(Point{10, 10});
    Shape* inCopy = copy.getShapeUnderCursor(Point{10, 10});
    assert(inCopy != nullptr && "the copy has the shapes too");
    assert(inCopy != inOriginal && "the copy owns new shapes, not the same pointers");

    inCopy->moveBy(1000, 0);
    assert(original.getShapeUnderCursor(Point{10, 10}) == inOriginal && "moving a shape in the copy leaves the original alone");
}

void testLayerAssignmentIsDeep() {
    Layer original = makeOverlappingLayer();

    Layer target{"Detalii"};
    target.addShape(std::make_unique<Ellipse>(makeStyle(), Point{300, 300}, 50, 50));

    target = original;
    assert(target.getName() == "Fundal" && "assignment copies the name");
    assert(target.getShapeUnderCursor(Point{325, 325}) == nullptr && "assignment drops the old shapes");

    Shape* inTarget = target.getShapeUnderCursor(Point{10, 10});
    assert(inTarget != nullptr && inTarget != original.getShapeUnderCursor(Point{10, 10})
           && "assignment clones the shapes");

    //atribuire catre sine
    Layer& alias = original;
    original = alias;
    assert(original.getShapeUnderCursor(Point{10, 10}) != nullptr && "self-assignment keeps the shapes");
}

void testLayerNames() {
    Layer layer{"Fundal"};
    layer.rename("Cer");
    assert(layer.getName() == "Cer" && "rename changes the name");

    bool threw = false;
    try {
        layer.rename("   ");
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "rename rejects a blank name");
    assert(layer.getName() == "Cer" && "a failed rename keeps the old name");

    threw = false;
    try {
        const Layer bad{""};
        (void) bad;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "the constructor rejects an empty name");
}

void testLayerPrint() {
    const Layer layer = makeOverlappingLayer();
    std::ostringstream out;
    out << layer;
    assert(out.str().find("Layer: Fundal") == 0 && "layer prints its name first");
    assert(out.str().find("Rectangle") != std::string::npos && "layer prints its shapes");
}