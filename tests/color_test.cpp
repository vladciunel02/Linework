//
// Created by vlad on 10/25/26.
//
#undef NDEBUG
#include <cassert>
#include <sstream>
#include <stdexcept>
#include "color_test.h"
#include "Color.h"

void testColorValid_1() {
    try{
        Color c1("#FF5733");
        std::ostringstream out;
        out << c1;
        assert(out.str() == "Color(R: 255, G: 87, B: 51)" && "Color output is not correct");
    }
    catch (const std::invalid_argument& ) {
        assert(false && "Valid color format should not throw an exception");
    }
}
void testColorValid_2() {
    try{
        Color c2("#00FF00");
        std::ostringstream out;
        out << c2;
        assert(out.str() == "Color(R: 0, G: 255, B: 0)" && "Color output is not correct");
    }
    catch (const std::invalid_argument& ) {
        assert(false && "Valid color format should not throw an exception");
    }
}

void testColorMissingHash() {
    bool threw = false;
    try {
        Color c3("FF5733");
        (void) c3; // ca sa nu tipe testele ca c3 nu e folosit
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "Invalid color format should throw an exception");
}

void testColorInvalidLength() {
    bool threw = false;
    try {
        Color c4("#FF573");
        (void) c4; // ca sa nu tipe testele ca c4 nu e folosit
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "Invalid color format should throw an exception");
}
void testColorInvalidCharacters() {
    bool threw = false;
    try {
        Color c5("#FF57G3");
        (void) c5; // ca sa nu tipe testele ca c5 nu e folosit
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "Invalid color format should throw an exception");
}
void testColorLowerCase() {
    try{
        Color c6("#ff5733");
        std::ostringstream out;
        out << c6;
        assert(out.str() == "Color(R: 255, G: 87, B: 51)" && "Color output is not correct");
    }
    catch (const std::invalid_argument& ) {
        assert(false && "Valid color format should not throw an exception");
    }
}