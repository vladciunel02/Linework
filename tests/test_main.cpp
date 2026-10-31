// Main ships no test framework on purpose: plain assert / static_assert only.
// Full framework examples live on tests/gtest and tests/Boost-ext-ut branches.

// assert() disappears when NDEBUG is defined (Release, RelWithDebInfo): the
// binary would exit 0 having checked nothing, and CI would call that a pass.
// Keep the checks on in every build type.
#undef NDEBUG
#include <cassert>
#include <stdexcept>

#include "point_test.h"
#include "color_test.h"
#include "style_test.h"
#include "shape_test.h"
#include "layer_test.h"
#include "canvas_test.h"
#include "drawing_test.h"
#include "editor_test.h"

// checked through the public interface only: the helper behind the invariant is
// file-local in src/Example.cpp and cannot even be named here
int main()
{
  
    // testing the point methods in mainTest

    testPointDistance();
    testPointMove();

    // testing the color methods in mainTest
    testColorValid_1();
    testColorValid_2();
    testColorMissingHash();
    testColorInvalidLength();
    testColorInvalidCharacters();
    testColorLowerCase();

    // testing the style methods in mainTest

    testStyleConstructor();
    testStyleBadColor();
    testStyleBadStrokeWidth();

    // testing the shape methods in mainTest
    testRectangleContainsPoint();
    testEllipseContainsPoint();
    testShapeMove();
    testShapePrint();
    testShapeInvalidDimensions();


    // testing the layer methods in mainTest
    testLayerTopmostShape();
    testLayerReorder();
    testLayerCopyIsDeep();
    testLayerAssignmentIsDeep();
    testLayerNames();
    testLayerPrint();


    // testing the canvas methods in mainTest
    testConstructorValid();
    testCanvasContainsPointOutside();

    //testing drawing methods
    testDrawingLoadFromFile();
    testDrawingShapeUnderCursor();
    testDrawingLayers();
    testDrawingInvalid();

    //testing editor methods
    testEditorSelect();
    testEditorMove();
    testEditorFront();
    testEditorNoSelection();
    testEditorRun();
    return 0;
}
