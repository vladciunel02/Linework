//
// Created by vlad on 10/27/26.
//
#include "Drawing.h"
#include "internal/NameHelper.h"
#include <stdexcept>
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

std::ostream& operator<<(std::ostream& os, const Drawing& drawing) {
    os << "Drawing: " << drawing.name << "\n";
    os << drawing.canvas << "\n";
    for(const auto& layer : drawing.layers) {
        os << layer << "\n";
    }
    return os;
}