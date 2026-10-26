//
// Created by vlad on 10/26/26.
//
#include "Style.h"

#include <iostream>
#include <stdexcept>
#include <cmath>
Style::Style(const Color &fillColor, const Color &strokeColor, double strokeWidth)
    : fillColor(fillColor), strokeColor(strokeColor), strokeWidth(strokeWidth) {
        //cheking the strokeWidth value ( to also pass NaN < 0)
        if(strokeWidth < 0 || std::isnan(strokeWidth)) {
            throw std::invalid_argument("Sroke width must be a non-negative number.");
        }
    }

std::ostream& operator<<(std::ostream& os, const Style& style) {
    os << "Fill: " << style.fillColor << ", Stroke: " << style.strokeColor << ", Width: " << style.strokeWidth;
    return os;
}