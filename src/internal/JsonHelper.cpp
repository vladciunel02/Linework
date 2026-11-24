//
// Created by vlad on 11/24/26.
//
#include "JsonHelper.h"
 Point readPoint(const nlohmann::json& json) { //cautam in json inauntru shape acolo unde e position
        double x = json.at("x").get<double>();
        double y = json.at("y").get<double>();
        return Point(x, y);
    }
    Color readColor(const nlohmann::json& json) {
        return Color(json.get<std::string>());
    }
    Style readStyle(const nlohmann::json& json){ //cautam in json inuntru shape acolo unde e style
        Color fillColor = readColor(json.at("fill"));
        Color strokeColor = readColor(json.at("stroke"));
        double strokeWidth = json.at("strokeWidth").get<double>();
        return Style(fillColor, strokeColor, strokeWidth);
    }
   
    