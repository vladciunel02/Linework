//
// Created by vlad on 10/27/26.
//
#include "Layer.h"
#include <stdexcept>
#include <cctype>
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

void Layer::rename(const std::string &newName){
    if(!isValidName(newName)){
        throw std::invalid_argument("Invalid layer name");
    }
    name = newName;
}

Shape* Layer::getShapeUnderCursor(const Point &cursorPosition) {
    for(auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
        if((*it)->containsPoint(cursorPosition)) {
            return it->get();
        }
    }
    return nullptr;
}

void Layer::bringToFront(Shape* shape){
    for(auto it = shapes.begin(); it != shapes.end(); ++it) {
        if(it->get() == shape) {
            std::unique_ptr<Shape> temp = std::move(*it);
            shapes.erase(it);
            shapes.push_back(std::move(temp));
            return;
        }
    }
        throw std::invalid_argument("Shape not found in the layer");
    
}

void Layer::sendToBack(Shape* shape){
    for(auto it = shapes.begin(); it != shapes.end(); ++it) {
        if(it->get() == shape) {
            std::unique_ptr<Shape> temp = std::move(*it);
            shapes.erase(it);
            shapes.insert(shapes.begin(), std::move(temp));
            return;
        }
    }
    throw std::invalid_argument("Shape not found in the layer");
}

void Layer::addShape(std::unique_ptr<Shape> shape) {
    if(!shape) {
        throw std::invalid_argument("Cannot add a null shape");
    }
    shapes.push_back(std::move(shape));
}

 std::ostream& operator<<(std::ostream& os, const Layer& layer) {
    os << "Layer: " << layer.name << "\n";
    for(const auto& shape : layer.shapes) {
        os << *shape << "\n";
    }
    return os;
}

Layer &Layer::operator=(const Layer& other) {
    if(this == &other) {
        return *this;
    }
    name = other.name;
    shapes.clear();
    for(const auto& shape : other.shapes) {
        shapes.push_back(shape->clone());
    }
    return *this;
}
