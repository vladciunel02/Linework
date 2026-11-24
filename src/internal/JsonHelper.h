//
// Created by vlad on 11/24/26.
//

#ifndef PROIECTPOO_JSONHELPER_H
#define PROIECTPOO_JSONHELPER_H
#include "Point.h"
#include "Style.h"
#include "Shape.h"
#include <json/json.hpp>
#include <memory>
    Point readPoint(const nlohmann::json& json);
    Color readColor(const nlohmann::json& json);
    Style readStyle(const nlohmann::json& json);
     template<typename T>
    std::unique_ptr<T> readBoxShape(const nlohmann::json& json){
        Point position = readPoint(json.at("position"));
        Style style = readStyle(json.at("style"));
        double width = json.at("width").get<double>();
        double height = json.at("height").get<double>();
        return std::make_unique<T>(style, position, width, height);
    }
    
#endif //PROIECTPOO_JSONHELPER_H
