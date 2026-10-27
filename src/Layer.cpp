//
// Created by vlad on 10/27/26.
//
#include "Layer.h"
#include <algorithm>
bool Layer::isValidName(const std::string& name) {
    for(char c : name){
        if(!std::isspace(static_cast<unsigned char>(c))) {
            return true;
        }
    }
    return false;
}
Layer::Layer(const std::string& name) : name(name) {
    if(!isValidName(name)) {
        throw std::invalid_argument("Invalid layer name");
    }
}
Layer::Layer(const Layer& other) : name(other.name) {
    for(const auto& shape : other.shapes) {
        shapes.push_back(shape->clone());
    }
}
