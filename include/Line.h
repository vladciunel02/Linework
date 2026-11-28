//
// Created by vlad on 11/27/26.
//
#ifndef PROIECTPOO_LINE_H
#define PROIECTPOO_LINE_H
#include "Shape.h"
#include <json/json.hpp>
class Line : public Shape {
public:
    enum class Diagonal {
        TopLeftToBottomRight,
        BottomLeftToTopRight
    };
private:
    Diagonal lineDirection;
public:
    Line(const Style &style, const Point &position, double width, double height, Diagonal direction);
    bool containsPoint(const Point &point, double tolerance) const override;
    std::unique_ptr<Shape> clone() const override;
    double perimeter() const override;
    double area() const override;
    std::string getShapeType() const override { return "Line"; }
    void validateSize(double newWidth, double newHeight) const override;
    static std::unique_ptr<Line> fromJson(const nlohmann::json &json);

    
};

#endif //PROIECTPOO_LINE_H