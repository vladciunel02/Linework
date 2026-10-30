//
// Created by vlad on 10/30/26.
//
#include "Editor.h"
#include <stdexcept>
#include <sstream>

Editor::Editor(const Drawing& drawing) : drawing(drawing){}
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
    else {
        throw std::invalid_argument("Unknown action: " + firstWord);
    }
}

void Editor::print(std::ostream& os) const {
    os << drawing;
}