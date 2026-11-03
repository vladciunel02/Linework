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
    std::tuple<std::string, double, double, double, double, std::string, std::string, double> readShapeParameters(std::istringstream& iss, const std::string& actionName) {
        std::string layerName;
        double x = 0;
        double y = 0;
        double width = 0;
        double height = 0;
        std::string fillColor;
        std::string strokeColor;
        double strokeWidth = 0;

        if(!(iss >> layerName >> x >> y >> width >> height >> fillColor >> strokeColor >> strokeWidth)) {
            throw std::invalid_argument("Invalid shape parameters for " + actionName + " action");
        }
        return std::make_tuple(layerName, x, y, width, height, fillColor, strokeColor, strokeWidth);
    }
    bool isModifyingAction(const std::string& action) {
    // comenzile care schimba desenul si pot fi anulate cu undo
    static const std::array<std::string, 19> modifyingActions = {
        "move", "moveto", "resize", "front", "back", "delete",
        "fill", "stroke", "strokewidth", "style",
        "rect", "ellipse",
        "add", "remove", "rename", "duplicate", "hide", "show",
        "load"
    };
    return std::ranges::find(modifyingActions, action) != modifyingActions.end();
}
}
Editor::Editor(const Drawing& drawing) : drawing(drawing){}

void Editor::requireSelection() const {
    if(!selectedShape || selectedLayerName.empty()) {
        throw std::invalid_argument("No shape selected");
    }
}
void Editor::clearSelection() {
    selectedShape = nullptr;
    selectedLayerName.clear();
}
void Editor::executeAction(const std::string& action, std::ostream& os){
    std::istringstream iss(action);
    std::string firstWord;
    iss >> firstWord; //ne ia primul cuvant din actiune pana da de spatiu , care e comanda
    if(firstWord.empty()){
        throw std::invalid_argument("Action cannot be empty");
    }
    const bool modifies = isModifyingAction(firstWord);
    const Drawing beforeAction = drawing; // salvam starea curenta a desenului
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
    else if(firstWord == "rect"){
        auto [layerName, x, y, width, height, fillColor, strokeColor, strokeWidth] = readShapeParameters(iss, firstWord);
        const Point position(x, y);
        const Style style(Color(fillColor), Color(strokeColor), strokeWidth);
        Editor::createAndSelectShape(layerName, "rectangle", style, position, width, height, os);
    }
    else if(firstWord == "ellipse"){
        auto [layerName, x, y, width, height, fillColor, strokeColor, strokeWidth] = readShapeParameters(iss, firstWord);
        const Point position(x, y);
        const Style style(Color(fillColor), Color(strokeColor), strokeWidth);
        Editor::createAndSelectShape(layerName, "ellipse", style, position, width, height, os);
    }
    else if(firstWord == "hide" || firstWord == "show"){
    std::string layerName;
    if(!(iss >> layerName)) {
        throw std::invalid_argument("Invalid layer name for " + firstWord + " action");
    }
    Editor::setLayerVisibility(layerName, firstWord == "show", os);
    }
    else if(firstWord == "layers"){
    Editor::listLayers(os);
    }
    else if(firstWord == "info"){
    Editor::info(os);
    }
    else if(firstWord == "undo"){
        Editor::undo(os);
    }
    else if(firstWord == "redo"){
        Editor::redo(os);
    }
    else if(firstWord == "help"){
        Editor::help(os);
    }
    else if(firstWord == "stats"){
        Editor::stats(os);
    }
    else {
        throw std::invalid_argument("Unknown action: " + firstWord);
    }
    if(modifies) {
        undoStack.push_back(beforeAction);
        redoStack.clear(); 
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
void Editor::createAndSelectShape(const std::string& layerName, const std::string& type, const Style& style,
                                     const Point& position, double width, double height, std::ostream& os){
    Shape* newShape = drawing.createShape(layerName, type, style, position, width, height);
    selectedShape = newShape;
    selectedLayerName = layerName;
    os << "Created and selected new shape: " << *newShape << " in layer: " << layerName << '\n';
}
void Editor::setLayerVisibility(const std::string& layerName, bool visible, std::ostream& os){
    drawing.getLayer(layerName).setVisible(visible);
    // daca ascundem layerul formei selectate, golim selectia
    if(!visible && selectedLayerName == layerName) {
        selectedShape = nullptr;
        selectedLayerName.clear();
    }
    os << (visible ? "Shown layer: " : "Hidden layer: ") << layerName << '\n';
}
void Editor::listLayers(std::ostream& os) const {
    const auto& layers = drawing.getLayers();
    if(layers.empty()) {
        os << "No layers in the drawing" << '\n';
        return;
    }
    os << "Layers (bottom to top):" << '\n';
    int index = 1;
    for(const auto& layer : layers) {
        os << index << ". " << layer.getName()
           << " - " << layer.getShapes().size() << " shape(s)";
        if(!layer.isVisible()) {
            os << " (hidden)";
        }
        // marcam layerul formei selectate
        if(layer.getName() == selectedLayerName) {
            os << " <- selected";
        }
        os << '\n';
        ++index;
    }
}
void Editor::info(std::ostream& os) const {
    requireSelection();
    os << "Selected shape: " << *selectedShape << '\n';
    os << "Type: " << selectedShape->getShapeType() << '\n';
    os << "Layer: " << selectedLayerName << '\n';
}
void Editor::print(std::ostream& os) const {
    os << drawing;
}
void Editor::undo(std::ostream& os){
    if(undoStack.empty()) {
        throw std::invalid_argument("Nothing to undo");
    }
    redoStack.push_back(drawing); // salvam starea curenta pentru redo
    drawing = undoStack.back(); // revenim la starea anterioara
    undoStack.pop_back();
    clearSelection();
    os << "Undo performed" << '\n';
}
void Editor::redo(std::ostream& os){
    if(redoStack.empty()) {
        throw std::invalid_argument("Nothing to redo");
    }
    undoStack.push_back(drawing); // salvam starea curenta pentru undo
    drawing = redoStack.back(); 
    redoStack.pop_back();
    clearSelection();
    os << "Redo performed" << '\n';
}
void Editor::stats(std::ostream& os) const {
    const auto& layers = drawing.getLayers();
    int totalShapes = 0;
    int totalRectangles = 0;
    int totalEllipses = 0;
    int shownLayers = 0;
    int hiddenLayers = 0;
    for(const auto& layer : layers) {
        totalShapes += layer.getShapes().size();
        if(layer.isVisible()) {
            ++shownLayers;
        } else {
            ++hiddenLayers;
        }
        for(const auto& shapePtr : layer.getShapes()) {
            if(shapePtr->getShapeType() == "rectangle") {
                ++totalRectangles;
            } else if(shapePtr->getShapeType() == "ellipse") {
                ++totalEllipses;
            }
        }
    }
    os << "Drawing: " << drawing.getName() << '\n';
    os << "Total layers: " << layers.size() << '\n';
    os << "Shown layers: " << shownLayers << '\n';
    os << "Hidden layers: " << hiddenLayers << '\n';
    os << "Total shapes: " << totalShapes << '\n';
    os << "Total rectangles: " << totalRectangles << '\n';
    os << "Total ellipses: " << totalEllipses << '\n';
}
void Editor::help(std::ostream& os){
    os << "Available commands:\n";
    os<< "  help - Show this help message\n";
    os<< " stats - Show statistics about the drawing\n";
    os << "  print - Print the current drawing\n";
    os << "  select x y - Select a shape at the given coordinates\n";
    os << "  move dx dy - Move the selected shape by the given amounts\n";
    os << "  moveto x y - Move the selected shape to the given coordinates\n";
    os << "  front - Bring the selected shape to the front\n";
    os << "  back - Send the selected shape to the back\n";
    os << "  duplicate existingLayer newLayer - Duplicate a layer\n";
    os << "  rename oldName newName - Rename a layer\n";
    os << "  remove layerName - Remove a layer\n";
    os << "  add layerName - Add a new layer\n";
    os << "  delete - Delete the selected shape\n";
    os << "  resize newWidth newHeight - Resize the selected shape\n";
    os << "  save filename - Save the current drawing to a file\n";
    os << "  load filename - Load a drawing from a file\n";
    os << "  fill color - Fill the selected shape with the given color\n";
    os << "  stroke color - Stroke the selected shape with the given color\n";
    os << "  strokewidth width - Set the stroke width of the selected shape\n";
    os << "  style fillColor strokeColor strokeWidth - Set the style of the selected shape\n";
    os << "  rect layerName x y width height fillColor strokeColor strokeWidth - Create a rectangle\n";
    os << "  ellipse layerName x y width height fillColor strokeColor strokeWidth - Create an ellipse\n";
    os << "  hide layerName - Hide a layer\n";
    os << "  show layerName - Show a layer\n";
    os << "  layers - List all layers\n";
    os << "  info - Show information about the selected shape\n";
    os << "  undo - Undo the last action\n";
    os << "  redo - Redo the last undone action\n";
}