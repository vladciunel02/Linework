// Main ships no test framework on purpose: plain assert / static_assert only.
// Full framework examples live on tests/gtest and tests/Boost-ext-ut branches.

// assert() disappears when NDEBUG is defined (Release, RelWithDebInfo): the
// binary would exit 0 having checked nothing, and CI would call that a pass.
// Keep the checks on in every build type.
#undef NDEBUG
#include <cassert>
#include <stdexcept>

#include "Example.h" // the library under test; include/ comes from oop
#include "point_test.h"
#include "color_test.h"
#include "style_test.h"
#include "shape_test.h"
static_assert(Example::limit > 0, "the invariant below assumes a positive limit");

// checked through the public interface only: the helper behind the invariant is
// file-local in src/Example.cpp and cannot even be named here
int main()
{
    Example e;
    assert(e.value() == 0);

    e.add(7);
    assert(e.value() == 7);
    e.add(-7);
    assert(e.value() == 0);

    bool threw = false;
    try
    {
        e.add(-1);
    }
    catch (const std::invalid_argument &)
    {
        threw = true;
    }
    assert(threw && "add() must refuse to go below 0");
    assert(e.value() == 0 && "a failed call leaves the object untouched");

    threw = false;
    try
    {
        e.add(Example::limit + 1);
    }
    catch (const std::invalid_argument &)
    {
        threw = true;
    }
    assert(threw && "add() must refuse to go above limit");

    threw = false;
    try
    {
        Example broken{Example::limit + 1};
        (void)broken;
    }
    catch (const std::invalid_argument &)
    {
        threw = true;
    }
    assert(threw && "the constructor must not hand out a broken object either");

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

    return 0;
}
