//
// Created by vlad on 11/28/26.
//
#include "Line.h"
#include <stdexcept>
#include "internal/JsonHelper.h"
namespace{
    Line::Diagonal diagonalFromString(const std::string& directionStr) {
        if (directionStr == "TopLeftToBottomRight") {
            return Line::Diagonal::TopLeftToBottomRight;
        } else if (directionStr == "BottomLeftToTopRight") {
            return Line::Diagonal::BottomLeftToTopRight;
        } else {
            throw std::invalid_argument("Invalid line direction: " + directionStr);
        }
    }
    std::string diagonalToString(Line::Diagonal direction) {
        switch (direction) {
            case Line::Diagonal::TopLeftToBottomRight:
                return "TopLeftToBottomRight";
            case Line::Diagonal::BottomLeftToTopRight:
                return "BottomLeftToTopRight";
            default:
                throw std::invalid_argument("Invalid line direction");
        }
    }
}
Line::Line(const Style &style, const Point &position, double width, double height, Diagonal direction)
    : Shape(style, position, width, height), lineDirection(direction) {
        validateSize(width, height);
    }

std::unique_ptr<Shape> Line::clone() const{
    return std::make_unique<Line>(*this);
}
double Line::area() const {
    return 0.0;
}
double Line::perimeter() const {
    return std::sqrt(getWidth() * getWidth() + getHeight() * getHeight());
}
std::unique_ptr<Line> Line::fromJson(const nlohmann::json &json) {
    const Style style = readStyle(json.at("style"));
    const Point position = readPoint(json.at("position"));
    const double width = json.at("width").get<double>();
    const double height = json.at("height").get<double>();
    const std::string directionStr = json.at("direction").get<std::string>();
    const Diagonal direction = diagonalFromString(directionStr);
    return std::make_unique<Line>(style, position, width, height, direction);

}
void Line::validateSize(double newWidth, double newHeight) const{
    if(newWidth < 0 || newHeight < 0) {
        throw std::invalid_argument("Width and height must be non-negative for a line.");
    }
}