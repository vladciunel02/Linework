//
// Created by vlad on 10/27/26.
//

#ifndef PROIECTPOO_CANVAS_H
#define PROIECTPOO_CANVAS_H
#include "Point.h"
class Canvas{
    private:
    int width;
    int height;

    public:
    static constexpr int maxWidth = 16384; // 16384 e valoare rezonabila 
    static constexpr int maxHeight = 16384; //fiind dimensiunea maxima pe care multe placi grafice o pot suporta
    Canvas(int width, int height);
    bool containsPoint (const Point& point) const;
    friend std::ostream& operator<<(std::ostream& os, const Canvas& canvas);

};
#endif //PROIECTPOO_CANVAS_H
