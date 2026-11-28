//
// Created by vlad on 10/27/26.
//

#ifndef PROIECTPOO_ELLIPSE_H
#define PROIECTPOO_ELLIPSE_H
#include "Shape.h"
#include <json/json.hpp>
class Ellipse : public Shape{
public:
    Ellipse(const Style &style, const Point &position, double width, double height);
    bool containsPoint(const Point &point, double tolerance) const override;
    std::unique_ptr<Shape> clone() const override;
    double perimeter() const override;
    double area() const override;
    static std::unique_ptr<Ellipse> fromJson(const nlohmann::json &json);
protected:
    std::string getShapeType() const override { return "Ellipse"; }
};

#endif //PROIECTPOO_ELLIPSE_H
