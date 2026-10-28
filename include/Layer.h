//
// Created by vlad on 10/27/26.
//

#ifndef PROIECTPOO_LAYER_H
#define PROIECTPOO_LAYER_H
#include <string>
#include "Shape.h"
#include <vector>
#include <memory>
class Layer{
    private:
    std::string name;
    std::vector<std::unique_ptr<Shape>> shapes;
    
    public:
    explicit Layer(const std::string& name);
    //copy contructor pentru duplicate layer
    Layer(const Layer& other);
    Layer& operator=(const Layer& other);
    const std::string& getName() const{return name;}
    void rename(const std::string &name); 
    void addShape(std::unique_ptr<Shape> shape);
    void bringToFront(const Shape* shape);
    void sendToBack(const Shape* shape);
    Shape* getShapeUnderCursor(const Point &cursorPosition);
    friend std::ostream& operator<<(std::ostream& os, const Layer& layer);
    Layer& operator=(const Layer& other);

    ~Layer() = default;
    
};
#endif //PROIECTPOO_LAYER_H
