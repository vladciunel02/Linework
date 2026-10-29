#include <iostream>
#include <array>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>

#include "Point.h"
#include "Style.h"
#include "Color.h"



int main() {
    


    //declaram niste puncte
    Point p1(1, 2);
    Point p2(3, 4);
    //le afisam in consola valoriile sa vedem daca merge
    std::cout << p1 << "\n";
    std::cout << p2 << "\n";

    //apelam si niste metode
    double dist = p1.distanceTo(p2);

    //afisam distanta
    std::cout<<dist<<"\n";

    //mutam si un punct
    Point p3 = p1.moveBy(1, 2);

    //il afisam
    std::cout << p3 << "\n";

    //declaram niste culori
    Color c1("#00ff00"); //culoare verde
    Color c2("#ff0000"); //culoare rosie
    Color c3("#0000ff"); //culoare albastra
    //afisam culorile
    std::cout << c1 << "\n";
    std::cout << c2 << "\n";
    std::cout << c3 << "\n";


    //declaram un stil
    Style s1(c1, c2, 2.5); //fillColor = verde, strokeColor = rosie, strokeWidth = 2.5
    //afisam stilul
    std::cout << s1 << "\n";



    sf::RenderWindow window;
    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    window.create(sf::VideoMode({800, 700}), "My Window", sf::Style::Default);
    ///////////////////////////////////////////////////////////////////////////
    std::cout << "Fereastra a fost creată\n";
    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: mandatory use one of vsync or FPS limit (not both)            ///
    /// This is needed so we do not burn the GPU                            ///
    window.setVerticalSyncEnabled(true);                                    ///
    /// window.setFramerateLimit(60);                                       ///
    ///////////////////////////////////////////////////////////////////////////

    while(window.isOpen()) {
        bool shouldExit = false;

        while(const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                std::cout << "Fereastra a fost închisă\n";
            }
            else if (event->is<sf::Event::Resized>()) {
                std::cout << "New width: " << window.getSize().x << '\n'
                          << "New height: " << window.getSize().y << '\n';
            }
            else if (event->is<sf::Event::KeyPressed>()) {
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                std::cout << "Received key " << (keyPressed->scancode == sf::Keyboard::Scancode::X ? "X" : "(other)") << "\n";
                if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    shouldExit = true;
                }
            }
        }
        if(shouldExit) {
            window.close();
            std::cout << "Fereastra a fost închisă (shouldExit == true)\n";
            break;
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(300ms);

        window.clear();
        window.display();
    }

    std::cout << "Programul a terminat execuția\n";
    return 0;
}
