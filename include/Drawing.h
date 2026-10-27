//
// Created by vlad on 10/27/26.
//

#ifndef PROIECTPOO_DRAWING_H
#define PROIECTPOO_DRAWING_H
#include "Layer.h"
#include <vector>
class Drawing{
private:
    std::vector<Layer> layers;
    std::string name;
    static bool isValidName(const std::string& name);
};
#endif //PROIECTPOO_DRAWING_H
