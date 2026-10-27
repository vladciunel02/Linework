//
// Created by vlad on 10/27/26.
//

#ifndef PROIECTPOO_RECTANGLE_H
#define PROIECTPOO_RECTANGLE_H
#include "Shape.h"
class Rectangle : public Shape {
public:
    Rectangle(const Style &style, const Point &position, double width, double height);
    bool containsPoint(const Point &point) const override;
    std::unique_ptr<Shape> clone() const override;
protected:
    std::string getShapeType() const override { return "Rectangle"; }
    
};
#endif //PROIECTPOO_RECTANGLE_H
