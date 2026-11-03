    //
    // Created by vlad on 10/30/26.
    //

    #ifndef PROIECTPOO_EDITOR_H
    #define PROIECTPOO_EDITOR_H
    #include "Drawing.h"
    #include <string>
    #include <iostream>
    class Editor {
    private:
        Drawing drawing;
        Shape* selectedShape = nullptr;
        std::string selectedLayerName; //tinem numele si nu layer ul, pot aparea probleme de memorie
        std::vector<Drawing> undoStack;
        std::vector<Drawing> redoStack;
        void requireSelection() const;
        void clearSelection();
    public:
        explicit Editor(const Drawing& drawing);
        void executeAction(const std::string& action, std::ostream& os);
        void print(std::ostream& os) const;
        void select(const Point& cursorPosition, std::ostream& os);
        void move(const Point& offset, std::ostream& os);
        void moveTo(const Point& newPosition, std::ostream& os);
        void front(std::ostream& os);
        void back(std::ostream& os);
        void duplicate(const std::string& layerName, const std::string& newLayerName, std::ostream& os);
        void rename(const std::string& oldName, const std::string& newName, std::ostream& os);
        void remove(const std::string& layerName, std::ostream& os);
        void add(const std::string& layerName, std::ostream& os);
        void deleteShape(std::ostream& os);
        void resizeShape(double newWidth, double newHeight, std::ostream& os);
        void run(std::istream& is, std::ostream& os);
        void saveFile(const std::string& filename, std::ostream& os);
        void loadFile(const std::string& filename, std::ostream& os);
        void fillShape(const std::string& color, std::ostream& os);
        void strokeShape(const std::string& color, std::ostream& os);
        void strokeWidthShape(double width, std::ostream& os);
        void setStyleShape(const std::string& fillColor, const std::string& strokeColor, double strokeWidth, std::ostream& os);
        void createAndSelectShape(const std::string& layerName, const std::string& type, const Style& style,
                                     const Point& position, double width, double height, std::ostream& os);
        void setLayerVisibility(const std::string& layerName, bool visible, std::ostream& os);
        void listLayers(std::ostream& os) const;
        void info(std::ostream& os) const;
        static void help(std::ostream& os);
        void stats(std::ostream& os) const;
        void undo(std::ostream& os);
        void redo(std::ostream& os);
        void resizeCanvas(double newWidth, double newHeight, std::ostream& os);

    };
    #endif //PROIECTPOO_EDITOR_H
