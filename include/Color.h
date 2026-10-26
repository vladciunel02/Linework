//
// Created by vlad on 10/25/26.
//

#ifndef PROIECTPOO_COLOR_H
#define PROIECTPOO_COLOR_H
#include <cstdint>
#include <string>
#include <iostream>
class Color {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    static bool isValidColor(const std::string& hex);
    void setColorFromHex(const std::string& hex);
    public:
    explicit Color(const std::string& hex);
    friend std::ostream& operator<<(std::ostream& os, const Color& color);
    friend bool operator==(const Color& lhs, const Color& rhs);
};
#endif //PROIECTPOO_COLOR_H
