//
// Created by vlad on 10/27/26.
//

#include "NameHelper.h"
bool isValidName(const std::string& name) {
    for(char c : name){
        if(!std::isspace(static_cast<unsigned char>(c))) {
            return true;
        }
    }
    return false;
}