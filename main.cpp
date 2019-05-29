#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "toolbox.h"
#include "databattle.h"


int main()
{
    // Default SFML test program

    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    // Databattle testing
    DataBattle* testBattle = new DataBattle("TestBattle");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
