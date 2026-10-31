//
// Created by vlad on 10/31/26.
//
#undef NDEBUG
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>

#include "editor_test.h"
#include "Editor.h"

void testEditorSelect() {
    Editor editor{Drawing::loadFromFile("assets/drawing.json")};
    std::ostringstream out;

    // (300, 200): elipsa galbena e deasupra dreptunghiului rosu
    editor.executeAction("select 300 200", out);
    // (600, 400): panza goala
    editor.executeAction("select 600 400", out);

    const std::string text = out.str();
    assert(text.find("Ellipse") != std::string::npos && "select picks the shape on top");
    assert(text.find("No shape found") != std::string::npos && "a click on empty canvas selects nothing");
}

void testEditorMove() {
    Editor editor{Drawing::loadFromFile("assets/drawing.json")};
    std::ostringstream out;

    editor.executeAction("select 100 100", out);   // dreptunghiul rosu, cu coltul la (50, 50)
    editor.executeAction("move 30 10", out);

    assert(out.str().find("Rectangle at (80, 60)") != std::string::npos && "move adds the offset to the position");
}

void testEditorFront() {
    Editor editor{Drawing::loadFromFile("assets/drawing.json")};
    std::ostringstream out;

    editor.executeAction("select 100 100", out);   // dreptunghiul rosu
    editor.executeAction("front", out);

    std::ostringstream afterFront;
    editor.executeAction("select 300 200", afterFront);
    assert(afterFront.str().find("Rectangle") != std::string::npos && "after front the rectangle is on top");
}

void testEditorNoSelection() {
    Editor editor{Drawing::loadFromFile("assets/drawing.json")};
    std::ostringstream out;

    bool threw = false;
    try {
        editor.executeAction("move 10 10", out);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "move without a selected shape is rejected");
}

void testEditorRun() {
    Editor editor{Drawing::loadFromFile("assets/drawing.json")};
    std::istringstream commands{"select 100 100\nbla\nmove 30 10\n"};
    std::ostringstream out;

    editor.run(commands, out);

    const std::string text = out.str();
    assert(text.find("Error: Unknown action: bla") != std::string::npos && "run reports the wrong command");
    assert(text.find("Rectangle at (80, 60)") != std::string::npos && "run keeps going after an error");
}
