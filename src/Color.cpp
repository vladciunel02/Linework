#include "Color.h"
#include <stdexcept>
#include <cctype>

//
// Created by vlad on 10/25/26.
//
bool Color::isValidColor(const std::string&  hex) {
   if(hex.length() != 7 || hex[0] != '#') {
       return false;
   }
   for(std::size_t i = 1; i < hex.length(); i++) {
       if(!std::isxdigit(static_cast<unsigned char>(hex[i]))) {
           return false;
       }
   }
   return true;

}

void Color::setColorFromHex(const std::string& hex){
    if(!isValidColor(hex)) {
        throw std::invalid_argument("Invalid color format");
    }
    r = static_cast<std::uint8_t>(std::stoi(hex.substr(1, 2), nullptr, 16));
    g = static_cast<std::uint8_t>(std::stoi(hex.substr(3, 2), nullptr, 16));
    b = static_cast<std::uint8_t>(std::stoi(hex.substr(5, 2), nullptr, 16));
}

Color::Color(const std::string& hex) {
    if (!isValidColor(hex)) {
        throw std::invalid_argument("Invalid color format");
    }
    setColorFromHex(hex);
}

 std::ostream& operator<<(std::ostream& os, const Color& color) {
    os << "Color(R: " << static_cast<int>(color.r)
       << ", G: " << static_cast<int>(color.g)
       << ", B: " << static_cast<int>(color.b) << ")";
    return os;
}

