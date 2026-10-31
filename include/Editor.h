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
        void requireSelection() const;
    public:
        explicit Editor(const Drawing& drawing);
        void executeAction(const std::string& action, std::ostream& os);
        void print(std::ostream& os) const;
        void select(const Point& cursorPosition, std::ostream& os);
        void move(const Point& offset, std::ostream& os);
        void moveTo(const Point& newPosition, std::ostream& os);
        void front(std::ostream& os);
        void back(std::ostream& os);
    };
    #endif //PROIECTPOO_EDITOR_H
