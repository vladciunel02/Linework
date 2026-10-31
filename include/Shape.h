//
// Created by vlad on 10/26/26.
//

#ifndef PROIECTPOO_SHAPE_H
#define PROIECTPOO_SHAPE_H
#include "Style.h"
#include "Point.h"
#include <string>
#include <memory>
class Shape{
private:
    Style style;
    Point position;
    double width;
    double height;
public:
    void moveTo(const Point &newPosition);
    void moveBy(double xOffset, double yOffset);
    virtual bool containsPoint(const Point &point) const = 0;
    virtual std::unique_ptr<Shape> clone() const = 0;
    virtual ~Shape() = default;
    friend std::ostream& operator<<(std::ostream& os, const Shape& shape);
    double getWidth() const { return width; }
    double getHeight() const { return height; }
protected:
    Shape(const Style &style, const Point &position, double width, double height);
    virtual std::string getShapeType() const = 0;
    Point getPosition() const { return position; }
    

};
#endif //PROIECTPOO_SHAPE_H
