//
// Created by vlad on 11/24/26.
//

#ifndef PROIECTPOO_SHAPEFACTORY_H
#define PROIECTPOO_SHAPEFACTORY_H
#include "Shape.h"
#include <map>
#include <functional>
#include <json/json.hpp>
class ShapeFactory{
    static std::map<std::string, std::function<std::unique_ptr<Shape>(const nlohmann::json&)>> shapeCreators;
public:
    static std::unique_ptr<Shape> createShape(const nlohmann::json& json);
};
#endif //PROIECTPOO_SHAPEFACTORY_H