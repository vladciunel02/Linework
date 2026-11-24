//
// Created by vlad on 11/24/26.
//

#include "ShapeFactory.h"
#include "Ellipse.h"
#include "Rectangle.h"
#include <stdexcept>
std::map<std::string, std::function<std::unique_ptr<Shape>(const nlohmann::json&)>> ShapeFactory::shapeCreators = {
    {"Ellipse", Ellipse::fromJson},
    {"Rectangle", Rectangle::fromJson}
};
std::unique_ptr<Shape> ShapeFactory::createShape(const nlohmann::json& json){
    std::string type = json.at("type").get<std::string>();
    auto it = shapeCreators.find(type);
    if(it != shapeCreators.end()){
        return it->second(json);
    } else {
        throw std::invalid_argument("Unknown shape type: " + type);
    }
}