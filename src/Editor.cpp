//
// Created by vlad on 10/30/26.
//
#include "Editor.h"
#include <stdexcept>
#include <sstream>
#include <utility>
namespace{
    std::pair<double, double> readCoordinates(std::istringstream& iss,  const std::string& actionName) {
        double x = 0;
        double y = 0;
        if(!(iss >> x >> y)) {
            throw std::invalid_argument("Invalid coordinates for " + actionName + " action");
        }
        auto pair = std::make_pair(x, y);
        return pair;
    }
}
Editor::Editor(const Drawing& drawing) : drawing(drawing){}

void Editor::requireSelection() const {
    if(!selectedShape || selectedLayerName.empty()) {
        throw std::invalid_argument("No shape selected");
    }
}
void Editor::executeAction(const std::string& action, std::ostream& os){
    std::istringstream iss(action);
    std::string firstWord;
    iss >> firstWord; //ne ia primul cuvant din actiune pana da de spatiu , care e comanda
    if(firstWord.empty()){
        throw std::invalid_argument("Action cannot be empty");
    }
    if(firstWord == "print"){
        Editor::print(os); // se poate si fara prefix ca suntem in scope-ul clasei
    }
    else if(firstWord == "select"){
        auto [x, y] = readCoordinates(iss, firstWord);
        const Point cursorPosition(x, y);
        Editor::select(cursorPosition, os);
    }
    else if(firstWord == "move"){
        auto [x, y] = readCoordinates(iss, firstWord);
        const Point offset(x, y);
        Editor::move(offset, os);
    }
    else if(firstWord == "moveto"){
        auto [x, y] = readCoordinates(iss, firstWord);
        const Point newPosition(x, y);
        Editor::moveTo(newPosition, os);
    }
    else if(firstWord == "front"){
        Editor::front(os);
    }
    else if(firstWord == "back"){
        Editor::back(os);
    }
    else {
        throw std::invalid_argument("Unknown action: " + firstWord);
    }

}

void Editor::select(const Point& cursorPosition, std::ostream& os) {
    Layer* currentLayer = drawing.getLayerUnderCursor(cursorPosition);
    if(!currentLayer) {
        selectedShape = nullptr;
        selectedLayerName.clear();
        os << "No shape found under cursor" << '\n';
        return;
    }
    selectedShape = currentLayer->getShapeUnderCursor(cursorPosition);
    selectedLayerName = currentLayer->getName();
    os << "Selected shape at: " << *selectedShape << " in layer: " << selectedLayerName << '\n';
}
void Editor::move(const Point& offset, std::ostream& os) {
    requireSelection();
    selectedShape->moveBy(offset.getX(), offset.getY());
    os << "Moved shape to: " << *selectedShape << " in layer: " << selectedLayerName << '\n';
}
void Editor::moveTo(const Point& newPosition, std::ostream& os){
    requireSelection();
    selectedShape->moveTo(newPosition);
    os << "Moved shape to: " << *selectedShape << " in layer: " << selectedLayerName << '\n';
}
void Editor::front(std::ostream& os){
    requireSelection();
    Layer& layer = drawing.getLayer(selectedLayerName);
    layer.bringToFront(selectedShape);
    os << "Brought shape to front: " << *selectedShape << " in layer: " << selectedLayerName << '\n';
}
void Editor::back(std::ostream& os){
    requireSelection();
    Layer& layer = drawing.getLayer(selectedLayerName);
    layer.sendToBack(selectedShape);
    os << "Sent shape to back: " << *selectedShape << " in layer: " << selectedLayerName << '\n';
}

void Editor::print(std::ostream& os) const {
    os << drawing;
}