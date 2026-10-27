//
// Created by vlad on 10/27/26.
//

#ifndef PROIECTPOO_ELLIPSE_H
#define PROIECTPOO_ELLIPSE_H
#include "Shape.h"
class Ellipse : public Shape{
public:
    Ellipse(const Style &style, const Point &position, double width, double height);
    bool containsPoint(const Point &point) const override;
    std::unique_ptr<Shape> clone() const override;
protected:
    std::string getShapeType() const override { return "Ellipse"; }
};

#endif //PROIECTPOO_ELLIPSE_H
