//
// Created by vlad on 10/25/26.
//

#ifndef PROIECTPOO_STYLE_H
#define PROIECTPOO_STYLE_H
#include "Color.h"
class Style {
private:
    Color fillColor;
    Color strokeColor;
    double strokeWidth;
public:
    Style(const Color &fillColor, const Color &strokeColor, double strokeWidth);
    Color getFillColor() const { return fillColor; }
    Color getStrokeColor() const { return strokeColor; }
    double getStrokeWidth() const { return strokeWidth; }
    friend std::ostream& operator<<(std::ostream& os, const Style& style);
};

#endif //PROIECTPOO_STYLE_H
