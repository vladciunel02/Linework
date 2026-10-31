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
    std::pair<std::string, std::string> readLayerNames(std::istringstream& iss, const std::string& actionName) {
        std::string layerName1;
        std::string layerName2;
        if(!(iss >> layerName1 >> layerName2)) {
            throw std::invalid_argument("Invalid layer names for " + actionName + " action");
        }
        return std::make_pair(layerName1, layerName2);
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
    else if(firstWord == "duplicate"){
        auto [existingLayer, newLayer] = readLayerNames(iss, firstWord);
        Editor::duplicate(existingLayer, newLayer, os);
    }
    else if(firstWord == "rename"){
        auto [oldName, newName] = readLayerNames(iss, firstWord);
        Editor::rename(oldName, newName, os);
    }
    else if(firstWord == "remove"){
        std::string layerName;
        if(!(iss >> layerName)) {
            throw std::invalid_argument("Invalid layer name for remove action");
        }
        Editor::remove(layerName, os);
    }
    else if(firstWord == "add"){
        std::string layerName;
        if(!(iss >> layerName)) {
            throw std::invalid_argument("Invalid layer name for add action");
        }
        Editor::add(layerName, os);
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
void Editor::duplicate(const std::string& layerName, const std::string& newLayerName, std::ostream& os){
    const Layer& layer = drawing.getLayer(layerName);
    //folosim copy constructorul pentru a crea un nou layer cu aceleasi forme
    Layer newLayer(layer);
    newLayer.rename(newLayerName);
    drawing.addLayer(newLayer);
    //dam clear la selectia curenta pentru a evita probleme de memorie
    selectedShape = nullptr;
    selectedLayerName.clear();
    os << "Duplicated layer: " << layerName << " to new layer: " << newLayerName << '\n';
}
void Editor::rename(const std::string& oldName, const std::string& newName, std::ostream& os){
    drawing.renameLayer(oldName, newName);
    //daca layerul selectat e cel care a fost redenumit, actualizam numele
    if(selectedLayerName == oldName) {
        selectedLayerName = newName;
    }
    os << "Renamed layer: " << oldName << " to new layer: " << newName << '\n';
}
void Editor::remove(const std::string& layerName, std::ostream& os){
    drawing.removeLayer(layerName);
    ///dam clear la selectia curenta pentru a evita probleme de memorie 
    selectedShape = nullptr;
    selectedLayerName.clear();
    os << "Removed layer: " << layerName << '\n';
}
void Editor::add(const std::string& layerName, std::ostream& os){
    Layer newLayer(layerName);
    drawing.addLayer(newLayer);
    selectedShape = nullptr;
    selectedLayerName.clear();
    os << "Added new layer: " << layerName << '\n';
}
void Editor::run(std::istream& is, std::ostream& os){
    std::string command;
    while(std::getline(is, command)) {
        if(command.empty() && command.back() == '\r') {
            command.pop_back(); // skip empty lines
        }
        if(command.empty()) {
            continue; // skip empty lines
        }
        os << " > " << command << '\n';
        try {
            executeAction(command, os);
        }catch(const std::exception& e) {
            os << "Error: " << e.what() << '\n';
        }
    }
        
}
void Editor::print(std::ostream& os) const {
    os << drawing;
}