//
// Created by vlad on 10/30/26.
//
#include "Editor.h"
#include <stdexcept>
#include <sstream>
#include <utility>
#include <tuple>
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
    std::tuple<std::string, std::string, double> readStyle(std::istringstream& iss, const std::string& actionName) {
        std::string fillColor;
        std::string strokeColor;
        double strokeWidth = 0;
        if(!(iss >> fillColor >> strokeColor >> strokeWidth)) {
            throw std::invalid_argument("Invalid style parameters for " + actionName + " action");
        }
        return std::make_tuple(fillColor, strokeColor, strokeWidth);
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
    else if(firstWord == "delete"){
        Editor::deleteShape(os);
    }
    else if(firstWord == "resize"){
        auto[newWidth, newHeight] = readCoordinates(iss, firstWord);
        Editor::resizeShape(newWidth, newHeight, os);
    }
    else if(firstWord =="save"){
        std::string filename;
        if(!(iss >> filename)) {
            throw std::invalid_argument("Invalid filename for save action");
        }
        Editor::saveFile(filename, os);
    }
    else if(firstWord == "load"){
        std::string filename;
        if(!(iss >> filename)) {
            throw std::invalid_argument("Invalid filename for load action");
        }
        Editor::loadFile(filename, os);
    }
    else if(firstWord == "fill"){
        std::string color;
        if(!(iss >> color)) {
            throw std::invalid_argument("Invalid color for fill action");
        }
        Editor::fillShape(color, os);
    }
    else if(firstWord == "stroke"){
        std::string color;
        if(!(iss >> color)) {
            throw std::invalid_argument("Invalid color for stroke action");
        }
        Editor::strokeShape(color, os);
    }
    else if(firstWord == "strokewidth"){
        double width = 0;
        if(!(iss >> width)) {
            throw std::invalid_argument("Invalid width for strokewidth action");
        }
        Editor::strokeWidthShape(width, os);
    }
    else if(firstWord == "style"){
        auto [fillColor, strokeColor, strokeWidth] = readStyle(iss, firstWord);
        Editor::setStyleShape(fillColor, strokeColor, strokeWidth, os);
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
void Editor::deleteShape(std::ostream& os){
    requireSelection();
    Layer& layer = drawing.getLayer(selectedLayerName);
    os << "Deleted shape: " << *selectedShape << " from layer: " << selectedLayerName << '\n';
    layer.deleteShape(selectedShape);
    selectedShape = nullptr;
    selectedLayerName.clear();
}
void Editor::resizeShape(double newWidth, double newHeight, std::ostream& os){
    requireSelection();
    selectedShape->resize(newWidth, newHeight);
    os << "Resized shape: " << *selectedShape << " in layer: " << selectedLayerName << '\n';
}
void Editor::run(std::istream& is, std::ostream& os){
    std::string command;
    while(std::getline(is, command)) {
        if(!command.empty() && command.back() == '\r') {
            command.pop_back(); 
        }
        if(command.empty()) {
            continue; 
        }
        os << " > " << command << '\n';
        try {
            executeAction(command, os);
        }catch(const std::exception& e) {
            os << "Error: " << e.what() << '\n';
        }
    }
        
}
void Editor::saveFile(const std::string& filename, std::ostream& os){
   drawing.saveToFile(filename);
    os << "Saved drawing to file: " << filename << '\n';
}
void Editor::loadFile(const std::string& filename, std::ostream& os){
    drawing = Drawing::loadFromFile(filename);
    selectedShape = nullptr;
    selectedLayerName.clear();
    os << "Loaded drawing from file: " << filename << " (selection cleared)" << '\n';
}
void Editor::fillShape(const std::string& color, std::ostream& os){
    requireSelection();
    Color fillColor(color);
    selectedShape->setFillColor(fillColor);
    os << "Set fill color of shape: " << *selectedShape << " in layer: " << selectedLayerName << " to: " << fillColor << '\n';
}
void Editor::strokeShape(const std::string& color, std::ostream& os){
    requireSelection();
    Color strokeColor(color);
    selectedShape->setStrokeColor(strokeColor);
    os << "Set stroke color of shape: " << *selectedShape << " in layer: " << selectedLayerName << " to: " << strokeColor << '\n';
}
void Editor::strokeWidthShape(double width, std::ostream& os){
    requireSelection();
    selectedShape->setStrokeWidth(width);
    os << "Set stroke width of shape: " << *selectedShape << " in layer: " << selectedLayerName << " to: " << width << '\n';
}
void Editor::setStyleShape(const std::string& fillColor, const std::string& strokeColor, double strokeWidth, std::ostream& os){
    requireSelection();
    Color fill(fillColor);
    Color stroke(strokeColor);
    selectedShape->setStyle(fill, stroke, strokeWidth);
    os << "Set style of shape: " << *selectedShape << " in layer: " << selectedLayerName << " to fill: " << fill << ", stroke: " << stroke << ", stroke width: " << strokeWidth << '\n';
}
void Editor::print(std::ostream& os) const {
    os << drawing;
}