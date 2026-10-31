//
// Created by vlad on 10/27/26.
//

#ifndef PROIECTPOO_DRAWING_H
#define PROIECTPOO_DRAWING_H
#include "Layer.h"
#include <vector>
#include <string>
#include <ostream>
#include "Canvas.h"
class Drawing{
private:
    std::string name;
    Canvas canvas;
    std::vector<Layer> layers;
public:
    Drawing(const std::string& name, const Canvas& canvas);
    void addLayer(const Layer& layer);
    void removeLayer(const std::string& layerName);
    Layer& getLayer(const std::string& layerName);
    Shape* getShapeUnderCursor(const Point& cursorPosition);
    Layer* getLayerUnderCursor(const Point& cursorPosition);
    void renameLayer(const std::string& oldName, const std::string& newName);
    static Drawing loadFromFile(const std::string& filename);
    friend std::ostream& operator<<(std::ostream& os, const Drawing& drawing);
};
#endif //PROIECTPOO_DRAWING_H
