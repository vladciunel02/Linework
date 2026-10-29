//
// Created by vlad on 10/27/26.
//
#include "Drawing.h"
#include "internal/NameHelper.h"
#include <stdexcept>
#include <json/json.hpp>
#include <fstream>
#include "Shape.h"
#include "Ellipse.h"
#include "Rectangle.h"
namespace{
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
    std::unique_ptr<Shape> readShape(const nlohmann::json& json) {
        std::string type = json.at("type").get<std::string>();
        Point position = readPoint(json.at("position"));
        Style style = readStyle(json.at("style"));
        double width = json.at("width").get<double>();
        double height = json.at("height").get<double>();

        if(type == "Ellipse") {
            return std::make_unique<Ellipse>(style, position, width, height);
        } else if(type == "Rectangle") {
            return std::make_unique<Rectangle>(style, position, width, height);
        } 
        else {
            throw std::invalid_argument("Unknown shape type: " + type);
        }
    }
}
Drawing::Drawing(const std::string& name, const Canvas& canvas) : name(name), canvas(canvas) {
    if(!isValidName(name)) {
        throw std::invalid_argument("Invalid drawing name");
    }
}

void Drawing::addLayer(const Layer& layer) {
    for(const auto& existingLayer : layers) {
        if(existingLayer.getName() == layer.getName()) {
            throw std::invalid_argument("Layer with the same name already exists");
        }
    }
    layers.push_back(layer);
}
void Drawing::removeLayer(const std::string& layerName) {
    for(auto it = layers.begin(); it != layers.end(); ++it) {
        if(it->getName() == layerName) {
            layers.erase(it);
            return;
        }
    }
    throw std::invalid_argument("Layer not found");
}
Layer& Drawing::getLayer(const std::string& layerName) {
    for(auto& layer : layers){
        if(layer.getName() == layerName) {
            return layer;
        }
    }
    throw std::invalid_argument("Layer not found");
}
Shape* Drawing::getShapeUnderCursor(const Point& cursorPosition){
    if(!canvas.containsPoint(cursorPosition)) {
        return nullptr;
    }
    for(auto it = layers.rbegin(); it != layers.rend(); ++it) {
        Shape* shape = it->getShapeUnderCursor(cursorPosition);
        if(shape != nullptr) {
            return shape;
        }
    }
    return nullptr;
}

Drawing Drawing::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if(!file.is_open()){
        throw std::runtime_error("Could not open file" + filename);
    }
    nlohmann::json json = nlohmann::json::parse(file);
    std::string drawingName = json.at("name").get<std::string>(); // convert to std::string
    int canvasWidth = json.at("canvas").at("width").get<int>();
    int canvasHeight = json.at("canvas").at("height").get<int>();
    Canvas canvas(canvasWidth, canvasHeight);
    Drawing drawing(drawingName, canvas);
    for(const auto& layerJson : json.at("layers")) {
        std::string layerName = layerJson.at("name").get<std::string>();
        Layer layer(layerName);
        for(const auto& shapeJson : layerJson.at("shapes")) {
            std::unique_ptr<Shape> shape = readShape(shapeJson);
            layer.addShape(std::move(shape));
        }
        drawing.addLayer(layer);    
     }
     return drawing;
    }

std::ostream& operator<<(std::ostream& os, const Drawing& drawing) {
    os << "Drawing: " << drawing.name << "\n";
    os << drawing.canvas << "\n";
    for(const auto& layer : drawing.layers) {
        os << layer << "\n";
    }
    return os;
}