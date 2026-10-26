//
// Created by vlad on 10/27/26.
//

#ifndef PROIECTPOO_ELIPSE_H
#define PROIECTPOO_ELIPSE_H
#include "Shape.h"
class Ellipse : public Shape{
public:
    Ellipse(const Style &style, const Point &position, double width, double height);
    bool containsPoint(const Point &point) const override;
protected:
    std::string getShapeType() const override { return "Ellipse"; }
};

#endif //PROIECTPOO_ELIPSE_H
